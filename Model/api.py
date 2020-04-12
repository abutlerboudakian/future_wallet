from flask import Flask, request, jsonify
import requests
import json
import dbutil
from modelfactory import ModelFactory
from modelpack import ModelType

mEng = dbutil.connect_engine('modeldata')
uEng = dbutil.connect_engine('accountdata')
modeldir = ''
fips = pd.read_sql('SELECT ZIP, Lat, Long FROM FIPS', con=mEng)
app = Flask(__name__)
app.config["DEBUG"] = True
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
				'symbol': ticker,
				'apikey': 'BYK3GWD7674OE5J5'})

		stockData = json.loads(stockData.content)
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
	for val, loc in assets_data['res'].items():
		location = fips['ZIP' == loc]
		res.append((float(val), float(location['Lat'][0]), float(location['Long'][[0]])))
	for val, loc in assets_data['rents'].items():
		location = fips['ZIP' == loc]
		res.append((float(val), float(location['Lat'][0]), float(location['Long'][0])))
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
			+ assets_data['rm'] + ");")

		for ticker, shares in invests_data['stocks'].items():
			uConn.execute("INSERT INTO UserStocks VALUES ('" + data['userid'] + "', " + ticker + ", " + shares + ");")

		for val, loc in assets_data['res'].items():
			uConn.execute("INSERT INTO UserRes VALUES('" + data['userid'] + "', '" + loc + "', " + val + ");")

		for val, loc in assets_data['rents'].items():
			uConn.execute("INSERT INTO UserRents VALUES('" + data['userid'] + "', '" + loc + "', " + val + ");")


	return jsonify(response)

@app.route('/getInputs', methods=['GET'])
def getInputs():
	return "{}"

@app.route('/submitBudget', methods=['POST'])
def submitBudget():
	return "{}"

@app.route('/getBudget', methods=['GET'])
def getBudget():
	return "{}"

@app.route('/getAllBudgets', methods=['GET'])
def getAllBudgets():
	return "{}"

@app.route('/login', methods=['GET'])
def login():
	return "{}"

@app.route('/logout', methods=['POST'])
def logout():
	return "{}"

@app.route('/register', methods=['POST'])
def register():
	return "{}"

@app.route('/update', methods=['POST'])
def update():
	return "{}"

@app.route('/getIndustries', methods=['GET'])
def getIndustries():
	return "{}"

app.run()