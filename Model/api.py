from flask import Flask, request, jsonify

app = Flask(__name__)
app.config["DEBUG"] = True

@app.route('/submitInputs', methods=['POST'])
def submitInputs():
	return "{}"

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

@app.route('/getIndustries', methods=['GET'])
def getIndustries():
	return "{}"

app.run()