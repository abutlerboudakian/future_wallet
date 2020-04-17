
from modelpack import WageModel, InvestmentModel, AssetModel, ModelType
from datasetbuilder import DatasetBuilder

class ModelFactory:
	def __init__(self):
		pass

	def createModel(self, mType, **kwargs):
		if mType == ModelType.WAGES:
			if 'train' in kwargs and kwargs['train'] == True:
				if 'industryCode' in kwargs:
					return WageModel(industryCode=kwargs['industryCode'])
				else:
					return WageModel()
			else:
				if 'industryCode' in kwargs:
					return WageModel(industryCode=kwargs['industryCode'])
				return WageModel()
		elif mType == ModelType.INVESTS:
			if 'train' in kwargs:
				if 'tickers' in kwargs:
					return InvestmentModel(train=kwargs['train'], tickers=kwargs['tickers'])
				else:
					return InvestmentModel(train=kwargs['train'])
			else:
				if 'tickers' in kwargs:
					return InvestmentModel(tickers=kwargs['tickers'])
				else:
					return InvestmentModel()
		elif mType == ModelType.ASSETS:
			if 'train' in kwargs:
				return AssetModel(train=kwargs['train'])
			else:
				return AssetModel()
		else:
			raise 'Invalid model type specified. ' + mType


	def createAllModels(self):
		return (WageModel(DataSetBuilder(ModelType.WAGES)), InvestmentModel(DatasetBuilder(ModelType.INVESTS)), AssetModel(DataSetBuilder(ModelType.ASSETS)))