from flask import Flask, request, jsonify
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