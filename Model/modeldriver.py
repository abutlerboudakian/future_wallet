import modelfactory as mf
from models import ModelType

if __name__ == "__main__":
	wages = mf.createModel(ModelType.WAGES)
	investments = mf.createModel(ModelType.INVEST)
	assets = mf.createModel(ModelType.ASSETS)

	wages.train()
	investments.train()
	assets.train()

	wages.save('F:/ServerData/FutureWallet/models')
	investments.save('F:/ServerData/FutureWallet/models')
	assets.save('F:/ServerData/FutureWallet/models')