
from models import WageModel, InvestmentModel, AssetModel, ModelType
from datasetbuilder import DatasetBuilder

class ModelFactory:
	def __init__(self):
		dsb = DatasetBuilder()

	def createModel(self, mType, **kwargs):
		if mType == ModelType.WAGES:
			if 'industryCode' in kwargs:
				model = WageModel(dsb.getModelData(ModelType.WAGES, industryCode=kwargs['industryCode']))
			else:
				return new WageModel()
		elif mType == ModelType.INVESTS:
			model = InvestmentModel(dsb.getModelData(ModelType.INVESTS, args))
		elif mType == ModelType.ASSETS:
			model = AssetModel(dsb.getModelData(ModelType.ASSETS, args))
		else:
			raise 'Invalid model type specified.', mType

		return model

	def createAllModels(self)
		return (WageModel(dsb.getModelData(ModelType.WAGES)), InvestmentModel(dsb.getModelData(ModelType.INVESTS)), AssetModel(dsb.getModelData(ModelType.ASSETS)))