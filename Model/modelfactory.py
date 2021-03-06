# Import Statements
from modelpack import WageModel, InvestmentModel, AssetModel, ModelType
from datasetbuilder import DatasetBuilder

# Class definition
class ModelFactory:

    '''
        @params:
            mType - ModelType to create
            **kwargs:
                train -     Flag to enable or disable training
                tickers -   List of tickers to train for stock models
        @requires:  mType is a valid ModelType
        @modifies:  None
        @effects:   Creates and returns a model based off of the mType provided
        @returns:   New BaseModel
    '''
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

    '''
        @modifies:  None
        @effects:   Creates all three aggregate modeltypes and returns them packed in a tuple
        @returns:   Tuple containing WageModel, InvestmentModel, AssetModel
    '''
    def createAllModels(self):
        return (WageModel(DatasetBuilder(ModelType.WAGES)), InvestmentModel(DatasetBuilder(ModelType.INVESTS)), AssetModel(DatasetBuilder(ModelType.ASSETS)))