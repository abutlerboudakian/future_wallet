import modelfactory as mf
import modelpack
from modelpack import ModelType
from datasetbuilder import DatasetBuilder
import pandas as pd
import os
import dbutil

if __name__ == "__main__":
	print('Starting...')
	mfac = mf.ModelFactory()
	engine = dbutil.connect_engine('modeldata')
	savedir = 'F:/ServerData/FutureWallet/models/'
	print('DB Engine connected...')

	#industryCodes = None
	#tickers = None
	#with engine.connect() as conn:
		#industryCodes = pd.read_sql("SELECT DISTINCT IndustryCode FROM CBPIncome", con=conn)
		#tickers = pd.read_sql("WITH Tickers AS (SELECT DISTINCT Ticker FROM Stocks) SELECT TOP 20 PERCENT Ticker FROM Tickers ORDER BY newid();", con=conn)

	# indExclude = []
	# for fname in os.listdir(savedir):
	# 	if fname.startswith('wagemodel'):
	# 		indExclude.append(fname[9:])
	# industryCodes = industryCodes['IndustryCode'].tolist()
	# newIndCodes = []
	# for i in industryCodes:
	# 	if not i.replace('/', '') in indExclude:
	# 		newIndCodes.append(i)
	# industryCodes = newIndCodes
	# tickExclude = []
	# for fname in os.listdir(savedir):
	# 	if fname.startswith('stockmodel'):
	# 		tickExclude.append(fname[10:])
	# tickers = tickers['Ticker'].tolist()
	# newTicks = []
	# for t in tickers:
	# 	if not t in tickExclude:
	# 		newTicks.append(t)
	# tickers = newTicks
	# print('Industry Codes and Tickers loaded...')


	# for ticker in tickers:
	# 	print('Stock model ' + ticker + ' loaded')
	# 	stock = modelpack.StockModel(DatasetBuilder(ModelType.STOCKS, ticker=ticker))
	# 	print('Stock model ' + ticker + ' created...')
	# 	stock.train(50, 5000000)
	# 	print('Stock model ' + ticker + ' trained...')
	# 	stock.save(savedir, ticker)
	# 	print('Stock model ' + ticker + ' saved...')

	# for i in industryCodes:
	# 	print('Wage model ' + i + ' loaded')
	# 	w = mfac.createModel(ModelType.WAGES, train=True, industryCode=i)
	# 	print('Wage model ' + i + ' created...')
	# 	w.train(50, 5000000)
	# 	print('Wage model ' + i + ' trained...')
	# 	w.save(savedir)
	# 	print('Wage model ' + i + ' saved...')
	
	investments = mfac.createModel(ModelType.INVESTS, train=True)
	print('Investment models created...')
	assets = mfac.createModel(ModelType.ASSETS, train=True)
	print('Asset models saved...')	

	investments.train(50, 5000000)
	assets.train(50, 5000000)

	investments.save(savedir)
	print('Investment models saved...')
	assets.save(savedir)
	print('Asset models saved...')