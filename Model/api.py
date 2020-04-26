
# Import statements
from flask import Flask, request, jsonify, make_response
import requests
import json
import pandas as pd
import dbutil
import os
from modelfactory import ModelFactory
from modelpack import ModelType

# Global declarations
mEng = dbutil.connect_engine('modeldata')
uEng = dbutil.connect_engine('accountdata')
modeldir = '/home/butlea2/futurewallet/models/'
fips = pd.read_sql('SELECT ZIP, Lat, Long FROM FIPS', con=mEng)
app = Flask(__name__)
active_users = set()
mfac = ModelFactory()

'''
    Endpoint for submitting inputs
    @requires:  Incoming request must match specification, industryCode and any tickers must be valid
    @modifies:  Inputs, UserStocks, UserRes, UserRent SQL tables
    @effects:   Takes data passed in from JSON request body, feeds that data into ML model, stores raw inputs, and returns predictions
    @returns:   Response containing JSON prediction metrics
'''
@app.route('/submitInputs', methods=['POST'])
def submitInputs():
        response = {}
        data = request.get_json(force=True)
        if float(data['years']) == 0:
            return make_response(jsonify({'wages': 0, 'invests': 0, 'assets': 0, 'years': 0}))
        userid = data['userid'].replace("'", "''")
        wages_data = data['wages']
        invests_data = data['invests']
        assets_data = data['assets']
        wages = mfac.createModel(ModelType.WAGES)
        industry = wages_data['industryCode']
        resultset = mEng.connect().execute('SELECT DISTINCT Name, IndustryCode FROM NAICS')
        naics = pd.DataFrame(resultset.fetchall(), columns=resultset.keys())
        industryCode = naics.loc[naics['Name'] == industry, ['IndustryCode']].to_numpy()[0][0]
        wages.load(modeldir, industryCode=industryCode)

        if wages_data['hourly'] == True:
                income = float(wages_data['income'])*float(wages_data['hourspw'])*46
        else:
                income = float(wages_data['income'])    

        location = fips[fips['ZIP'] == wages_data['loc']]
        response['wages'] = wages.predict(float(income), float(location['Lat'].tolist()[0]), float(location['Long'].tolist()[0]), int(data['years']))

        tickers = [*invests_data['stocks']]
        invests = mfac.createModel(ModelType.INVESTS)
        invests.load(modeldir, tickers)
        stocks = []
        for ticker, shares in invests_data['stocks'].items():
                stockData = requests.get('https://www.alphavantage.co/query',
                        params={
                                'function': 'TIME_SERIES_DAILY',
                                'symbol': ticker,
                                'apikey': 'BYK3GWD7674OE5J5'})

                stockData = json.loads(stockData.content)
                if 'Time Series (Daily)' in stockData:
                    stocks.append((ticker,
                            float(stockData['Time Series (Daily)'][list(stockData['Time Series (Daily)'].keys())[0]]['4. close']),
                            float(shares)))
        response['invests'] = invests.predict(float(invests_data['savings']),
                float(invests_data['cd']),
                stocks,
                float(invests_data['bonds']),
                float(invests_data['tbonds']),
                int(data['years']))
        
        assets = mfac.createModel(ModelType.ASSETS)
        assets.load(modeldir)
        res = []
        rents = []
        for r in assets_data['res']:
                location = fips[fips['ZIP'] == r['loc']]
                if not location.empty:
                    res.append((float(r['value']), float(location['Lat'].tolist()[0]), float(location['Long'].tolist()[0])))
        for r in assets_data['rents']:
                location = fips[fips['ZIP'] == r['loc']]
                if not location.empty:
                    res.append((float(r['value']), float(location['Lat'].tolist()[0]), float(location['Long'].tolist()[0])))
        response['assets'] = assets.predict(res, rents, float(assets_data['rm']), int(data['years']))
        response['years'] = data['years']

        uEng.execute("DELETE FROM Inputs WHERE userid='" + userid + "';")
        uEng.execute("DELETE FROM UserStocks WHERE userid='" + userid + "';")
        uEng.execute("DELETE FROM UserRes WHERE userid='" + userid + "';")
        uEng.execute("DELETE FROM UserRent WHERE userid='" + userid + "';")

        uEng.execute("INSERT INTO Inputs VALUES ('" + userid + "', '" 
                + wages_data['industryCode'].replace("'", "''") + "', '"
                + wages_data['loc'] + "', "
                + str(wages_data['income']) + ", " 
                + str(int(wages_data['hourly'])) + ", " 
                + str(wages_data['hourspw']) + ", " 
                + str(invests_data['savings']) + ", " 
                + str(invests_data['cd']) + ", "
                + str(invests_data['bonds']) + ", "
                + str(invests_data['tbonds']) + ", "
                + str(assets_data['rm']) + ", "
                + str(data['years']) + ");")

        for ticker, shares in invests_data['stocks'].items():
                uEng.execute("INSERT INTO UserStocks VALUES ('" + userid + "', '" + ticker + "', " + str(shares) + ");")

        for r in assets_data['res']:
                uEng.execute("INSERT INTO UserRes VALUES('" + userid + "', '" + r['loc'] + "', " + str(r['value']) + ");")

        for r in assets_data['rents']:
                uEng.execute("INSERT INTO UserRent VALUES('" + userid + "', '" + r['loc'] + "', " + str(r['value']) + ");")

        return jsonify(response)

'''
    Endpoint for storing raw inputs
    @requires:  Incoming request must match specification
    @modifies:  Inputs, UserStocks, UserRes, UserRent SQL tables
    @effects:   Takes data passed in from JSON request body, stores raw inputs
    @returns:   Response with confirmation message
'''
@app.route('/storeInputs', methods=['POST'])
def storeInputs():
        data = request.get_json()
        wages_data = data['wages']
        invests_data = data['invests']
        assets_data = data['assets']
        userid = data['userid'].replace("'", "''")
        uEng.execute("DELETE FROM Inputs WHERE userid='" + userid + "';")
        uEng.execute("DELETE FROM UserStocks WHERE userid='" + userid + "';")
        uEng.execute("DELETE FROM UserRes WHERE userid='" + userid + "';")
        uEng.execute("DELETE FROM UserRent WHERE userid='" + userid + "';")

        uEng.execute("INSERT INTO Inputs VALUES ('" + userid + "', '"
                + wages_data['industryCode'].replace("'", "''") + "', '"
                + wages_data['loc'] + "', "
                + str(wages_data['income']) + ", "
                + str(int(wages_data['hourly'])) + ", "
                + str(wages_data['hourspw']) + ", "
                + str(invests_data['savings']) + ", "
                + str(invests_data['cd']) + ", "
                + str(invests_data['bonds']) + ", "
                + str(invests_data['tbonds']) + ", "
                + str(assets_data['rm']) + ", "
                + str(data['years']) + ");")

        for ticker, shares in invests_data['stocks'].items():
                uEng.execute("INSERT INTO UserStocks VALUES ('" + userid + "', '" + ticker + "', " + str(shares) + ");")

        for r in assets_data['res']:
                uEng.execute("INSERT INTO UserRes VALUES('" + userid + "', '" + r['loc'] + "', " + str(r['value']) + ");")

        for r in assets_data['rents']:
            uEng.execute("INSERT INTO UserRent VALUES('" + userid + "', '" + r['loc'] + "', " + str(r['value']) + ");")

        return make_response('Input stored', 200)

'''
    Endpoint for retrieving raw inputs
    @requires:  Incoming request must match specification
    @modifies:  None
    @effects:   Takes userid passed in from query parameters, returns raw inputs associated with that userid
    @returns:   Response containing JSON user raw inputs
'''
@app.route('/getInputs', methods=['GET'])
def getInputs():
        response = {}
        response['userid'] = request.args.get('userid')
        userid = request.args.get('userid').replace("'", "''")
        wages = {}
        invests = {}
        assets = {}
        inputs = pd.read_sql("SELECT * FROM Inputs WHERE userid='" + userid + "';", con=uEng)
        stocks = pd.read_sql("SELECT ticker, shares FROM UserStocks WHERE userid='" + userid + "';", con=uEng)
        res = pd.read_sql("SELECT loc, price AS value FROM UserRes WHERE userid='" + userid + "';", con=uEng)
        rents = pd.read_sql("SELECT loc, rent AS value FROM UserRent WHERE userid='" + userid + "';", con=uEng)
        
        if inputs.empty:
                wages['industryCode'] = ''
                wages['income'] = 0.0
                wages['loc'] = ''
                wages['hourly'] = bool(True)
                wages['hourspw'] = 0
                invests['savings'] = 0.0
                invests['cd'] = 0.0
                invests['bonds'] = 0.0
                invests['tbonds'] = 0.0
                assets['rm'] = 0.0
                response['years'] = 0
        else:
                wages['industryCode'] = inputs['industryCode'][0]
                wages['income'] = inputs['income'][0]
                wages['loc'] = inputs['loc'][0]
                wages['hourly'] = bool(inputs['hourly'][0])
                wages['hourspw'] = inputs['hourspw'][0]
                invests['savings'] = inputs['savings'][0]
                invests['cd'] = inputs['cd'][0]
                invests['bonds'] = inputs['bonds'][0]
                invests['tbonds'] = inputs['tbonds'][0]
                assets['rm'] = inputs['rm'][0]
                response['years'] = int(inputs['years'][0])

        response['wages'] = wages

        if stocks.empty:
                invests['stocks'] = {}
        else:
                invests['stocks'] = {tick:share for tick, share in zip(stocks['ticker'].tolist(), stocks['shares'].tolist())}

        response['invests'] = invests

        assets['res'] = {} if res.empty else res.to_dict('records')
        assets['rents'] = {} if rents.empty else rents.to_dict('records')

        response['assets'] = assets

        return make_response(jsonify(response), 200)

'''
    Endpoint for submitting budgets
    @requires:  Incoming request must match specification
    @modifies:  BudgetCategories SQL Table
    @effects:   Takes budget data passed in from request, stores it in database
    @returns:   Response containing JSON budgetid
'''
@app.route('/submitBudget', methods=['POST'])
def submitBudget():
        data = request.get_json(force=True)
        response = {}
        check = uEng.execute("SELECT * FROM BudgetCategories WHERE userid='" + data['userid'].replace("'", "''") +
            "' AND budgetid='" + data['budgetid'].replace("'", "''") + "';")
        b = check.fetchone()
        if b:
            return make_response("Budget already exists", 400)
        response['budgetid'] = data['budgetid']
        budget = {'catname': [], 'weight': []}
        for catname, weight in data['categories'].items():
                budget['catname'].append(catname)
                budget['weight'].append(weight)
        budget = pd.DataFrame(budget)
        budget['userid'] = data['userid']
        budget['budgetid'] = data['budgetid']
        budget.to_sql('BudgetCategories', con=uEng, index=False, if_exists='append')
        return jsonify(response)

'''
    Endpoint for retreiving budgets
    @requires:  Incoming request must match specification
    @modifies:  None
    @effects:   Takes userid and budgetid from query parameters, returns associated budget data
    @returns:   Response containing JSON budget data
'''
@app.route('/getBudget', methods=['GET'])
def getBudget():
        response = {}
        budget = pd.read_sql("SELECT budgetid, catname, weight FROM BudgetCategories WHERE userid='"
                + request.args.get('userid').replace("'", "''") + "' AND budgetid='" + request.args.get('budgetid').replace("'", "''") + "';", con=uEng)
        response['name'] = request.args.get('budgetid')
        response['categories'] = {catname:weight for catname, weight in zip(budget['catname'].astype(str).tolist(), budget['weight'].astype(float).tolist())}
        return make_response(response, 200)

'''
    Endpoint for retrieving all budgets for a user
    @requires:  Incoming request must match specification
    @modifies:  None
    @effects:   Takes userid from query parametes, returns associated budgetids
    @returns:   Response containing JSON list of budgetids
'''
@app.route('/getAllBudgets', methods=['GET'])
def getAllBudgets():
        response = {}
        budgets = pd.read_sql("SELECT DISTINCT budgetid FROM BudgetCategories WHERE userid='" + request.args.get('userid').replace("'", "''") + "';", con=uEng)
        if budgets.empty:
                response['budgets'] = []
        else:
                response['budgets'] = budgets['budgetid'].tolist()
        return make_response(jsonify(response), 200)

'''
    Endpoint for logging a user in
    @requires:  Incoming request must match specification
    @modifies:  active_users
    @effects:   Takes username and password from request JSON body, returns authentication status
    @returns:   Response containing confirmation of successful or failed login
'''
@app.route('/login', methods=['POST'])
def login():
        userid = request.authorization['username']
        password = request.authorization['password']
        results = uEng.execute("SELECT * FROM Users WHERE userid='" + userid + "';")
        user = results.fetchone()
        if user and user.password == password:
                active_users.add(userid)
                return make_response('Auth Succeeded', 200)
        return make_response('Bad Credentials', 403)

'''
    Endpoint for logging out a user
    @requires:  Incoming request must match specification
    @modifies:  active_users
    @effects:   Takes username from request JSON body, logs our user
    @returns:   Response containing confirmation of successful or failed logout
'''
@app.route('/logout', methods=['POST'])
def logout():
        data = request.get_json(force=True)
        try:
                active_users.remove(data['userid'])
                return make_response('Logout Succeeded', 200)
        except Exception:
                return make_response('User Not Found', 404)

'''
    Endpoint for registering a user
    @requires:  Incoming request must match specification
    @modifies:  Users SQL Table
    @effects:   Takes username and password from request JSON body and adds them to database
    @returns:   Response containing confirmation of successful or failed registration
'''
@app.route('/register', methods=['POST'])
def register():
        data = request.get_json(force=True)
        check = uEng.execute("SELECT * FROM Users WHERE userid='" + data['userid'] + "';")
        user = check.fetchone()
        if user:
                return make_response('User Already Exists', 400)
        else:
                uEng.execute("INSERT INTO Users VALUES ('" + data['userid'] + "', '" + data['password'] + "');")
                return make_response('Registration Completed Successfully', 200)

'''
    Endpoint for updating user information
    @requires:  Incoming request must match specification
    @modifies:  Users, Inputs, UserStocks, UserRes, UserRent, BudgetCategories SQL Tables
    @effects:   Takes userid and/or password from JSON request body and updates all user tables
    @returns:   Response containing confirmation of successful or failed updates
'''
@app.route('/update', methods=['POST'])
def update():
        data = request.get_json(force=True)
        userid = data['userid'].replace("'", "''")
        if 'userid' in data['updates'] and 'password' in data['updates']:
            updUserid = data['updates']['userid'].replace("'", "''")
            password = data['updates']['password'].replace("'", "''")
            uEng.execute("UPDATE Users SET userid='"
                    + updUserid
                    + "', password='"
                    + password
                    + "' WHERE userid='"
                    + userid + "';")
            uEng.execute("UPDATE Inputs SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE UserStocks SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE UserRes SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE UserRent SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE BudgetCategories SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
        elif 'userid' in data['updates']:
            updUserid = data['updates']['userid'].replace("'", "''")
            uEng.execute("UPDATE Users SET userid='"
                    + updUserid
                    + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE Inputs SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE UserStocks SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE UserRes SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE UserRent SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
            uEng.execute("UPDATE BudgetCategories SET userid='" + updUserid + "' WHERE userid='" + userid + "';")
        elif 'password' in data['updates']:
            password = data['updates']['password'].replace("'", "''")
            uEng.execute("UPDATE Users SET password='"
                    + password
                    + "' WHERE userid='" + userid + "';")
        else:
            return make_response('No Updates Provided', 400)
        return make_response('Account Updated', 200)

'''
    Endpoint for retrieving industry names that are available for prediction
    @requires:  Incoming request must match specification
    @modifies:  None
    @effects:   Creates list of industries that are trained and returns them
    @returns:   Response containing JSON list of trained industries
'''
@app.route('/getIndustries', methods=['GET'])
def getIndustries():
        response = {}
        industries = pd.read_sql("SELECT DISTINCT Name, IndustryCode FROM NAICS", con=mEng)
        indInclude = []
        for fname in os.listdir(modeldir):
            if fname.startswith('wagemodel'):
                indInclude.append(fname[9:])
        codes = industries['IndustryCode'].tolist()
        newCodes = []
        for c in codes:
            if c.replace('/', '') in indInclude:
                newCodes.append(c)
        codes = list(newCodes)


        response['industries'] = industries.loc[industries['IndustryCode'].isin(codes), 'Name'].tolist()
        return make_response(response, 200)

'''
    Endpoint for retrieving stock tickers that are available for prediction
    @requires:  Incoming request must match specification
    @modifies:  None
    @effects:   Creates list of stock tickers that are trained and returns them
    @returns:   Response containing JSON list of trained tickers
'''
@app.route('/getTickers', methods=['GET'])
def getTickers():
        response = {}
        tickInclude = []
        for fname in os.listdir(modeldir):
            if fname.startswith('stockmodel'):
                tickInclude.append(fname[10:])
        response['tickers'] = tickInclude
        return make_response(response, 200)

'''
    Endpoint for retrieving users that are currently logged in:
    @requires:  Incoming request must match specification
    @modifies:  None
    @effects:   Gets collection of active users in list format and returns them
    @returns:   Response containing JSON list of currently logged in users
'''
@app.route('/getActiveUsers', methods=['GET'])
def getActiveUsers():
        response = {}
        response['users'] = list(active_users)

        return make_response(response, 200)

# Driver Block for Debug
if __name__ == '__main__':
        app.run()
