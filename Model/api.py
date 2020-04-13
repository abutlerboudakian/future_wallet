from flask import Flask, request, jsonify, make_response
import requests
import json
import pandas as pd
import dbutil
from modelfactory import ModelFactory
from modelpack import ModelType

mEng = dbutil.connect_engine('modeldata')
uEng = dbutil.connect_engine('accountdata')
modeldir = ''
fips = pd.read_sql('SELECT ZIP, Lat, Long FROM FIPS', con=mEng)
app = Flask(__name__)
app.config["DEBUG"] = True
active_users = set()
mfac = ModelFactory()

@app.route('/submitInputs', methods=['POST'])
def submitInputs():
	response = {}
	data = request.get_json()
	wages_data = data['wages']
	invests_data = data['invests']
	assets_data = data['assets']
	wages = mfac.createModel(ModelType.WAGES)
	wages.load(modeldir, industryCode=wages_data['industryCode'])

	if wages_data['hourly'] == True:
		income = float(wages_data['income'])*float(wages_data['hourspw'])*46
	else:
		income = float(wages_data['income'])	

	location = fips['ZIP' == wages_data['loc']]
	response['wages'] = wages.predict(float(income), float(location['Lat'][0]), float(location['Long'][0]), int(data['years']))

	tickers = [*invests_data['stocks']]
	invests = mfac.createModel(ModelType.INVESTS)
	invests.load(modeldir, tickers)
	stocks = []
	for ticker, shares in invests_data['stocks'].items():
		stockData = requests.get('https://www.alphavantage.co/query',
			params={
				'function': 'TIME_SERIES_DAILY',
				'symbol': ticker.upper(),
				'apikey': 'BYK3GWD7674OE5J5'})

		stockData = json.loads(stockData.content)
		stocks.append((ticker.upper(),
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
		location = fips['ZIP' == r['loc']]
		res.append((float(r['value']), float(location['Lat'][0]), float(location['Long'][[0]])))
	for r in assets_data['rents']:
		location = fips['ZIP' == r['loc']]
		res.append((float(r['value']), float(location['Lat'][0]), float(location['Long'][0])))
	response['assets'] = assets.predict(res, rents, float(assets_data['rm']), int(data['years']))
	response['years'] = data['years']

	with uEng.connect() as uConn:
		uConn.execute("DELETE FROM Inputs WHERE userid='" + data['userid'] + "';")
		uConn.execute("DELETE FROM UserStocks WHERE userid='" + data['userid'] + "';")
		uConn.execute("DELETE FROM UserRes WHERE userid='" + data['userid'] + "';")
		uConn.execute("DELETE FROM UserRents WHERE userid='" + data['userid'] + "';")

		uConn.execute("INSERT INTO Inputs VALUES ('" + data['userid'] + "', '" 
			+ wages_data['industryCode'] "', " 
			+ wages_data['income'] + ", " 
			+ wages_data['hourly'].upper() + ", " 
			+ wages_data['hourspw'] + ", " 
			+ invests_data['savings'] + ", " 
			+ invests_data['cd'] + ", "
			+ invests_data['bonds'] + ", "
			+ invests_data['tbonds'] + ", "
			+ assets_data['rm'] + ", "
			+ data['years'] + ");")

		for ticker, shares in invests_data['stocks'].items():
			uConn.execute("INSERT INTO UserStocks VALUES ('" + data['userid'] + "', " + ticker.upper() + ", " + shares + ");")

		for r in assets_data['res']:
			uConn.execute("INSERT INTO UserRes VALUES('" + data['userid'] + "', '" + r['loc'] + "', " + r['value'] + ");")

		for r in assets_data['rents']:
			uConn.execute("INSERT INTO UserRents VALUES('" + data['userid'] + "', '" + r['loc'] + "', " + r['val'] + ");")

	return jsonify(response)

@app.route('/getInputs', methods=['GET'])
def getInputs():
	response = {}
	data = request.get_json()
	response['userid'] = data['userid']
	wages = {}
	invests = {}
	assets = {}
	with uEng.connect() as uConn:
		inputs = pd.read_sql("SELECT * FROM Inputs WHERE userid='" + data['userid'] + "';", con=uConn)
		stocks = pd.read_sql("SELECT ticker, shares FROM UserStocks WHERE userid='" + data['userid'] + "';", con=uConn)
		res = pd.read_sql("SELECT loc, price AS value FROM UserRes WHERE userid='" + data['userid'] + "';", con=uConn)
		rents = pd.read_sql("SELECT loc, rent AS value FROM UserRents WHERE userid='" + data['userid'] + "';", con=uConn)
	wages['industryCode'] = inputs['industryCode'][0]
	wages['income'] = inputs['income'][0]
	wages['loc'] = inputs['loc'][0]
	wages['hourly'] = inputs['hourly'][0]
	wages['hourspw'] = inputs['hourspw'][0]

	response['wages'] = wages

	invests['savings'] = inputs['savings'][0]
	invests['cd'] = inputs['cd'][0]
	invests['stocks'] = {tick:share for tick, share in (stocks['ticker'].tolist(), stocks['shares'].tolist())}
	invests['bonds'] = inputs['bonds'][0]
	invests['tbonds'] = inputs['tbonds'][0]

	response['invests'] = invests

	assets['res'] = res.to_dict('records')
	assets['rents'] = rents.to_dict('records')
	assets['rm'] = inputs['rm'][0]

	response['assets'] = assets

	return jsonify(response)

@app.route('/submitBudget', methods=['POST'])
def submitBudget():
	data = request.get_json()
	response = {}
	reponse['budgetid'] = data['budgetid']
	budget = {}
	for catname, weight in data['categories'].items():
		budget['catname'] = catname
		budget['weight'] = weight
	budget = pd.DataFrame(budget)
	budget['userid'] = data['userid']
	budget['budgetid'] = data['budgetid']
	with uEng.connect() as uConn:
		budget.to_sql('BudgetCategories', con=uConn, index=False, if_exists='append')
	return jsonify(response)

@app.route('/getBudget', methods=['GET'])
def getBudget():
	data = request.get_json()
	response = {}
	with uEng.connect() as uConn:
		budget = pd.read_sql("SELECT budgetid, catname, weight FROM BudgetCategories WHERE userid='"
			+ data['userid'] + "' AND budgetid='" + data['budgetid'] + "';", con=uConn)
	response['name'] = data['budgetid']
	response['categories'] = {catname:weight for catname, weight in (budget['catname'].tolist(), budget['weight'].tolist())}
	return jsonify(response)

@app.route('/getAllBudgets', methods=['GET'])
def getAllBudgets():
	data = request.get_json()
	response = {}
	with uEng.connect() as uConn:
		budgets = pd.read_sql("SELECT DISTINCT budgetid FROM BudgetCategories WHERE userid='" + data['userid'] + "';", con=uConn)
	response['budgets'] = budgets['budgetid'].tolist()
	return jsonify(response)

@app.route('/login', methods=['POST'])
def login():
	userid = request.authorization['username']
	password = request.authorization['password']
	with uEng.connect() as uConn:
		results = uConn.execute("SELECT * FROM Users WHERE userid='" + userid + "';")
		user = results.fetchone()
	if user and user.password == password:
		active_users.add(userid)
		return make_response('Auth Succeeded', 200)
	return make_response('Bad Credentials', 403)

@app.route('/logout', methods=['POST'])
def logout():
	data = request.get_json()
	try:
		active_users.remove(data['userid'])
		return make_response('Logout Succeeded', 200)
	except Exception:
		return make_response('User Not Found', 404)

@app.route('/register', methods=['POST'])
def register():
	data = request.get_json()
	with uEng.connect() as uConn:
		check = uConn.execute("SELECT * FROM Users WHERE userid='" + data['userid'] + "';")
		user = check.fetchone()
		if user:
			return make_response('User Already Exists', 400)
		else:
			uConn.execute("INSERT INTO Users VALUES ('" + data['userid'] + "', '" + data['password'] + "');")
			active_users.add(data['userid'])
			return make_response('Registration Completed Successfully', 200)

@app.route('/update', methods=['POST'])
def update():
	data = request.get_json()
	with uEng.connect() as uConn:
		if 'userid' in data['updates'] and 'password' in data['updates']:
			uConn.execute("UPDATE Users SET userid='"
				+ data['updates']['userid']
				+ "', password='"
				+ data['updates']['password']
				+ "' WHERE userid='"
				+ data['userid'] + "';")
		elif 'userid' in data['updates']:
			uConn.execute("UPDATE Users SET userid='"
				+ data['updates']['userid']
				+ "' WHERE userid='" + data['userid'] + "';")
		elif 'password' in data['updates']:
			uConn.execute("UPDATE Users SET password='"
				+ data['updates']['password']
				+ "' WHERE userid='" + data['userid'] + "';")
		else:
			return make_response('No Updates Provided', 400)
	return make_response('Account Updated', 200)

@app.route('/getIndustries', methods=['GET'])
def getIndustries():
	response = {}
	with mEng.connect() as mConn:
		industries = pd.read_sql("SELECT DISTINCT Name FROM NAICS ORDER BY IndustryCode ASC")

	response['industries'] = industries['Name'].tolist()
	return jsonify(response)

@app.route('/getTickers', methods=['GET'])
def getTickers():
	response = {}
	with mEng.connect() as mConn:
		tickers = pd.read_sql("SELECT DISTINCT Ticker FROM Stocks ORDER BY Ticker ASC")

	response['tickers'] = tickers['Ticker'].tolist()
	return jsonify(response)

app.run()