import modelfactory as mf
from models import ModelType
import pandas as pd
import dbutil

if __name__ == "__main__":
	print('Starting...')
	mfac = mf.ModelFactory()
	engine = dbutil.connect_engine('modeldata')
	print('DB Engine connected...')

	wages = []
	industryCodes = None
	with engine.connect() as conn:
		industryCodes = pd.read_sql("SELECT DISTINCT IndustryCode FROM CBPIncome", con=conn)

	industryCodes = industryCodes['IndustryCode'].tolist()
	print('Industry Codes loaded...')

	for i in industryCodes:
		wages.append(mfac.createModel(ModelType.WAGES, train=True, industryCode=i))
	print('Wages models created...')
	investments = mfac.createModel(ModelType.INVESTS, train=True)
	print('Investment models created...')
	assets = mfac.createModel(ModelType.ASSETS, train=True)
	print('Asset models saved...')

	for w in wages:
		w.train(50)
		wages.save('F:/ServerData/FutureWallet/models/')
	
	print('Wage models saved...')

	investments.train(50)
	assets.train(50)

	investments.save('F:/ServerData/FutureWallet/models/')
	print('Investment models saved...')
	assets.save('F:/ServerData/FutureWallet/models/')
	print('Asset models saved...')