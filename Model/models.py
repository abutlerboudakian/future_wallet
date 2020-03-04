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

	def predict(self, income, lat, lon, years):
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
		timestamp = datetime.now().timestamp()
		predTimestamp = (datetime.now() + timedelta(years=years)).timestamp()
		for ticker, stock, shares in stocks:
			predStock = stockModels[ticker].predict(timestamp, predTimestamp, stock)
			stockRet += ((predStock - stock)*shares)
		stockRet /= years
		return stockRet + savingsModel.predict(timestamp, predTimestamp, savings) + cdModel.predict(timestamp, predTimestamp, cd) + bondModel.predict(timestamp, predTimestamp, bonds) + tbModel.predict(timestamp, predTimestamp, tbs)
		

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
		timestamp = datetime.now().timestamp()
		predTimestamp = (datetime.now() + timedelta(years=years)).timestamp()
		for (res, lat, lon) in residences:
			predRes = residenceModel.predict(timestamp, predTimestamp, res, lat, lon)
			resRet += (predRes - res)/years
		for (rent, lat, lon) in rents:
			predRent = rentModel.predict(timestamp, predTimestamp, rent, lat, lon)
			rentRet += predRent*12
		return resRet + rentRet + rmModel.predict(timestamp, predTimestamp, rm)

class SavingsModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		pass

class CDModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		pass

class StockModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		pass

class BondModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		pass

class TBModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		pass

class ResidenceModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value, lat, long):
		pass

class RentModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value, lat, long):
		pass

class RMModel(BaseModel):
	def __init__(self, data=None):
		pass

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		pass