from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
from tensorflow import keras
import numpy as np
import pandas as pd

from abc import ABC
from enum import Enum
from datetime import datetime, timedelta

class ModelType(Enum):
	WAGES = 1
	INVESTS = 2
	ASSETS = 3

class BaseModel(ABC):
	def __init__(self, data=None):
		self.data = data

	def load(self, models):
		pass

class WageModel(BaseModel):
	def __init__(self, data=None):
		pass

class InvestmentModel(BaseModel):
	def __init__(self, data=None):
		savingsData, cdData, stockData, bondData, tbData = zip(*data)
		self.savingsModel = SavingsModel(savingsData)
		self.cdModel = CDModel(savingsData)
		self.stockModels = []
		for s in stockData:
			self.stockModels[s['ticker']] = StockModel(s)
		self.bondModel = BondModel(bondData)
		self.tbModel = TBModel(tbData)

	def load(self, models):
		pass

	def predict(self, savings, cd, stocks, bonds, tbs, years):
		stockRet = 0
		predTimestamp = (datetime.now() + timedelta(years=years)).timestamp()
		for ticker, stock, shares in stocks:
			predStock = stockModels[ticker].predict(predTimestamp, stock)
			stockRet += ((predStock - stock)*shares)
		stockRet /= years
		return stockRet + savingsModel.predict(predTimestamp, savings) + cdModel.predict(predTimestamp, cd) + bondModel.predict(predTimestamp, bonds) + tbModel.predict(predTimestamp, tbs)
		

class AssetModel(BaseModel):
	def __init__(self, data=None):
		resData, rentData, rmData = zip(*data)
		self.residenceModel = ResidenceModel(resData)
		self.rentModel = RentModel(rentData)
		self.rmModel = RMModel(rmData)

	def load(self, models):
		pass

	def predict(self, residences, rents, rm, years):
		resRet = 0
		rentRet = 0
		predTimestamp = (datetime.now() + timedelta(years=years)).timestamp()
		for res in residences:
			predRes = residenceModel.predict(predTimestamp, res)
			resRet += (predRes - res)/years
		for rent in rents:
			predRent = rentModel.predict(predTimestamp, rent)
			rentRet += predRent*12
		return resRet + rentRet + rmModel.predict(predTimestamp, rm)

class SavingsModel(BaseModel):
	pass

class CDModel(BaseModel):
	pass

class StockModel(BaseModel):
	pass

class BondModel(BaseModel):
	pass

class TBModel(BaseModel):
	pass

class ResidenceModel(BaseModel):
	pass

class RentModel(BaseModel):
	pass

class RMModel(BaseModel):
	pass