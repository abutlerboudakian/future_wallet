import modelfactory as mf
from modelpack import ModelType
import pandas as pd
import os
import dbutil

if __name__ == "__main__":
	print('Starting...')
	mfac = mf.ModelFactory()
	engine = dbutil.connect_engine('modeldata')
	savedir = 'F:/ServerData/FutureWallet/models/'
	print('DB Engine connected...')

	industryCodes = None
	tickers = None
	with engine.connect() as conn:
		industryCodes = pd.read_sql("SELECT DISTINCT IndustryCode FROM CBPIncome", con=conn)
		tickers = pd.read_sql("WITH Tickers AS (SELECT DISTINCT Ticker FROM Stocks) SELECT TOP 20 PERCENT Ticker FROM Tickers ORDER BY newid();", con=conn)

	indExclude = []
	for fname in os.listdir(savedir):
		if fname.startswith('wagemodel'):
			indExclude.append(fname[9:])
	industryCodes = industryCodes['IndustryCode'].tolist()
	newIndCodes = []
	for i in industryCodes:
		if not i.replace('/', '') in indExclude:
			newIndCodes.append(i)
	industryCodes = newIndCodes
	tickers = tickers['Ticker'].tolist()
	print('Industry Codes and Tickers loaded...')


	for i in industryCodes:
		print('Wage model ' + i + ' loaded')
		w = mfac.createModel(ModelType.WAGES, train=True, industryCode=i)
		print('Wage model ' + i + ' created...')
		w.train(50, 1000000)
		print('Wage model ' + i + ' trained...')
		w.save(savedir)
		print('Wage model ' + i + ' saved...')
	
	investments = mfac.createModel(ModelType.INVESTS, train=True)
	print('Investment models created...')
	assets = mfac.createModel(ModelType.ASSETS, train=True)
	print('Asset models saved...')	

	investments.train(50, 1000000)
	assets.train(50, 1000000)

	investments.save(savedir)
	print('Investment models saved...')
	assets.save(savedir)
	print('Asset models saved...')