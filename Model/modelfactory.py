
from models import WageModel, InvestmentModel, AssetModel, ModelType
from datasetbuilder import DatasetBuilder

class ModelFactory:
	def __init__(self):
		self.dsb = DatasetBuilder()

	def createModel(self, mType, **kwargs):
		if mType == ModelType.WAGES:
			if 'train' in kwargs and kwargs['train'] == True:
				if 'industryCode' in kwargs:
					return WageModel(self.dsb.getModelData(ModelType.WAGES, industryCode=kwargs['industryCode']))
				else:
					return WageModel(self.dsb.getModelData(ModelType.WAGES))
			else:
				return WageModel()
		elif mType == ModelType.INVESTS:
			if 'train' in kwargs and kwargs['train'] == True:
				return InvestmentModel(self.dsb.getModelData(ModelType.INVESTS))
			else:
				return InvestmentModel()
		elif mType == ModelType.ASSETS:
			if 'train' in kwargs and kwargs['train'] == True:
				return AssetModel(self.dsb.getModelData(ModelType.ASSETS))
			else:
				return AssetModel()
		else:
			raise 'Invalid model type specified. ' + mType


	def createAllModels(self):
		return (WageModel(dsb.getModelData(ModelType.WAGES)), InvestmentModel(dsb.getModelData(ModelType.INVESTS)), AssetModel(dsb.getModelData(ModelType.ASSETS)))