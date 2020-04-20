from flask import Flask, escape, request, jsonify

app = Flask("FutureWalletServer")

@app.route("/", methods=["GET"])
def ping():
  print(request.args.get("userid"), request.args.get("budgetid"), flush=True)
  return "Ping Success"

"""@app.route("/predict", methods=["GET", "POST"])
def predict():
  print(request.get_json(), flush=True)
  return jsonify({"BudgetName":"Dang", "Categories":["thing", "thing2"], "Values":[0.3, 0.7]}, 200)"""

@app.route("/submitInputs", methods=["POST"])
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
  data = request.get_json();
  if (data["budgetid"] != "Orange Budget" or 
      data["userid"] != "Dummy" or 
      data["categories"]["Can Food"] != 0.5 or 
      data["categories"]["Boxed Food"] != 0.5):
      return jsonify({}, 405)
  return jsonify({"budgetid":data["budgetid"]}, 200)

@app.route("/getBudget", methods=["GET"])
def getBudget():
  if (request.args.get("budgetid") != "Orange Things" or
      request.args.get("userid") != "Dummy"):
      return jsonify({}, 405)
  return ({"name":"Orange Things", "categories":{"Oranges":0.5, "Fanta":0.25, "Tangerines":0.25}}, 200)

@app.route("/getAllBudgets", methods=["GET"])
def getAllBudgets():
  if (request.args.get("userid") != "Dummy"):
    return jsonify({}, 405)
  return ({"budgets":["Orange Budget", "Apple Budget"]}, 200)

@app.route("/register", methods=["POST"])
def register():
  data = request.get_json()
  if (data["userid"] != "Dummy" or
      data["password"] != "0f961dce6a9c174213b5891e4cb14bab"):
      return jsonify({}, 405)
  return "Registration Completed Successfully"

@app.route("/update", methods=["POST"])
def update():
  data = request.get_json()
  if (data["userid"] != "Dummy"):
      return jsonify({}, 405)
  elif (data["updates"].get("userid") and data["updates"]["userid"] == "Rabbit" and data["updates"].get("password") and data["updates"]["password"] == "4a69c68f08d1b8c27ec877484d8eaa16"):
      print("Changed Name and Password", flush=True)
  elif (data["updates"].get("userid") and data["updates"]["userid"] == "Rabbit"):
      print("Changed Name", flush=True)
  elif (data["updates"].get("password") and data["updates"]["password"] == "4a69c68f08d1b8c27ec877484d8eaa16"):
      print("Password", flush=True)
  return ({}, 200)

@app.route("/getIndustries", methods=["GET"])
def getIndustries():
  return ({"industries":["Orange Peeling", "Lychee Farmer", "World Destroyer"]}, 200)

@app.route("/getTickers", methods=["GET"])
def getTickers():
  return ({"tickers":["APPL", "GOOGL", "TSLA"]}, 200)

@app.route("/login", methods=["POST"])
def login():
  data = request.authorization
  if (data["username"] != "Dummy" or
      data["password"] != "0f961dce6a9c174213b5891e4cb14bab"):
      return jsonify({}, 405)
  return "Auth Succeeded"

@app.route("/logout", methods=["POST"])
def logout():
  if (request.get_json()["userid"] != "Dummy"):
      return jsonify({}, 405)
  return ({}, 200)

@app.route("/getInputs", methods=["GET"])
def getInputs():
  if (request.args.get("userid") != "Dummy"):
    return jsonify({}, 405)
  return ({
    "assets": {
        "rents": [{
            "loc": "12180",
            "value": 1
        }],
        "res": [{
            "loc": "12180",
            "value": 2
        }],
        "rm": 3
    },
    "invests": {
        "bonds": 4,
        "cd": 5,
        "savings": 6,
        "stocks": {
            "a": 7
        },
        "tbonds": 8
    },
    "userid": "Dummy",
    "wages": {
        "hourly": True,
        "hourspw": 9,
        "income": 10,
        "industryCode": " Abrasive Product Manufacturing",
        "loc": "12180"
    },
    "years": 11
}, 200)