import modelfactory as mf
from modelpack import ModelType
import pandas as pd
import dbutil

if __name__ == '__main__':
	mfac = mf.ModelFactory()

	wages = mfac.createModel(ModelType.WAGES, industryCode='212')

	wages.load('F:/ServerData/FutureWallet/models/', industryCode='212')

	prediction = wages.predict(500000, 42.7, -78.671, 30)

	print(prediction)