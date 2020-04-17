import modelfactory as mf
from modelpack import ModelType
import pandas as pd
import dbutil

if __name__ == '__main__':
	mfac = mf.ModelFactory()

	wages = mfac.createModel(ModelType.WAGES, industryCode='213')

	wages.load('F:/ServerData/FutureWallet/models/', industryCode='213')

	prediction = wages.predict(50000, 42.7, -78.671, 3)

	print(prediction)