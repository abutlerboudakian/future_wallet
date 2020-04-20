import modelfactory as mf
from modelpack import ModelType
import pandas as pd
import dbutil

if __name__ == '__main__':
	mfac = mf.ModelFactory()

	wages = mfac.createModel(ModelType.WAGES, industryCode='5152')

	wages.load('F:/ServerData/FutureWallet/models/', industryCode='5152')

	prediction = wages.predict(50000, 42.7, -78.671, 3)
	print(prediction)

	investments = mfac.createModel(ModelType.INVESTS)
	investments.load('F:/ServerData/FutureWallet/models/', tickers=['cern'])
	prediction = investments.predict(500, 5000, [('cern', 34.5, 2)], 400, 40000, 2)

	print(prediction)

	assets = mfac.createModel(ModelType.ASSETS)
	assets.load('F:/ServerData/FutureWallet/models/')
	prediction = assets.predict([(40000, 42.7, -78.671)], [(1500, 42.7, -78.671)], 1000, 1)

	print(prediction)