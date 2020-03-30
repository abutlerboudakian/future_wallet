import modelfactory as mf
from models import ModelType
import pandas as pd
import dbutil

if __name__ == "__main__":
	mfac = mf.ModelFactory()
	engine = dbutil.connect_engine('modeldata')

	wages = []
	industryCodes = None
	with engine.connect() as conn:
		industryCodes = pd.read_sql("SELECT DISTINCT IndustryCode FROM CBPIncome", con=conn)

	industryCodes = industryCodes['IndustryCode'].aslist()
	
	for i in industryCodes:
		wages.append(mfac.createModel(ModelType.WAGES, train=True, industryCode=i))
	investments = mfac.createModel(ModelType.INVESTS, train=True)
	assets = mfac.createModel(ModelType.ASSETS, train=True)

	for w in wages:
		w.train(50)
		wages.save('F:/ServerData/FutureWallet/models/')
	
	investments.train(50)
	assets.train(50)

	investments.save('F:/ServerData/FutureWallet/models/')
	assets.save('F:/ServerData/FutureWallet/models/')