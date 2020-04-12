from flask import Flask, request, jsonify
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
	stocks = {}
	response['invests'] = invests.predict(float(invests_data['savings']), float(invests_data['cd']), stocks, float(invests_data['bonds']), float(invests_data['tbonds']), int(data['years']))
	
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