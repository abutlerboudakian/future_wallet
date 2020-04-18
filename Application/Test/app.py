from flask import Flask, escape, request, jsonify

app = Flask("FutureWalletServer")

@app.route("/", methods=["GET"])
def ping():
  print(request.args.get("userid"), request.args.get("budgetid"), flush=True)
  return "Ping Success"

@app.route("/predict", methods=["GET", "POST"])
def predict():
  print(request.get_json(), flush=True)
  return jsonify({"BudgetName":"Dang", "Categories":["thing", "thing2"], "Values":[0.3, 0.7]}, 200)

@app.route("/storeInputs", methods=["POST"])
def submitInputs():
  data = request.get_json()
  if (data["years"] != 1 or
      data["wages"]["hourly"] != True or
      data["wages"]["hourspw"] != 9 or
      data["wages"]["income"] != 10 or
      data["wages"]["industryCode"] != " Abrasive Product Manufacturing" or
      data["wages"]["loc"] != "33333" or
      data["invests"]["bonds"] != 4 or
      data["invests"]["cd"] != 5 or
      data["invests"]["savings"] != 6 or
      data["invests"]["stocks"]["a"] != 7 or
      data["invests"]["tbonds"] != 8 or
      data["assets"]["rents"][0]["loc"] != "11111" or
      data["assets"]["rents"][0]["value"] != 1 or
      data["assets"]["res"][0]["loc"] != "22222" or
      data["assets"]["res"][0]["value"] != 2 or
      data["assets"]["rm"] != 3):
      return jsonify({}, 405)
  return jsonify({"wages":100, "invests":200, "assets":300, "years":1}, 200)

@app.route("/submitBudget", methods=["POST"])
def submitBudget():
  print(request.get_json(), flush=True)
  return request.get_json()

@app.route("/getBudget", methods=["GET"])
def getBudget():
  print(request.get_json(), flush=True)
  return ({"name":"Budget1", "categories":{"Cat1":0.5, "Cat2":0.4}}, 200)

@app.route("/getAllBudgets", methods=["GET"])
def getAllBudgets():
  print(request.get_json(), flush=True)
  return ({"budgets":["Budget1", "Budget 10000"]}, 200)

@app.route("/register", methods=["POST"])
def register():
  print(request.get_json(), flush=True)
  return ({}, 200)

@app.route("/update", methods=["POST"])
def update():
  print(request.get_json(), flush=True)
  return ({}, 200)

@app.route("/getIndustries", methods=["GET"])
def getIndustries():
  print(request.get_json(), flush=True)
  return ({"industries":["Hello", "Software", "Chungus"]}, 200)

@app.route("/getTickers", methods=["GET"])
def getTickers():
  print(request.get_json(), flush=True)
  return ({"tickers":["tock", "tick", "Chun"]}, 200)

@app.route("/login", methods=["POST"])
def login():
  print(request.get_json(), flush=True)
  return ({}, 200)

@app.route("/logout", methods=["POST"])
def logout():
  print(request.get_json(), flush=True)
  return ({}, 200)

@app.route("/getInputs", methods=["GET"])
def getInputs():
  print(request.get_json(), flush=True)
  return ({
    "assets": {
        "rents": [{
            "loc": "",
            "value": 0
        }],
        "res": [{
            "loc": "",
            "value": 0
        }],
        "rm": 0
    },
    "invests": {
        "bonds": 0,
        "cd": 0,
        "savings": 0,
        "stocks": [{
            "a": 0
        }],
        "tbonds": 0
    },
    "userid": "Hello WOrld",
    "wages": {
        "hourly": true,
        "hourspw": 0,
        "income": 0,
        "industryCode": " Abrasive Product Manufacturing",
        "loc": ""
    },
    "years": 7536745
}, 200)