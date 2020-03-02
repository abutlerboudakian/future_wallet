from abc import ABC
from enum import Enum

class ModelType(Enum):
	WAGES = 1
	INVESTS = 2
	ASSETS = 3

class BaseModel(ABC):
	pass

class WageModel(BaseModel):
	pass

class InvestmentModel(BaseModel):
	pass

class AssetModel(BaseModel):
	pass