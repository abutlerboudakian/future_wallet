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
		self.model = keras.Sequential([
				keras.layers.Dense(len(data['X'][0]), input_shape=(len(data['X'][0]),)),
				keras.layers.Dense(len(data['X'][0])+1, activation='relu'),
				keras.layers.Dense(1)
			])

	def setData(self, data):
		self.data = data

	def train(self, epochs):
		x_train, x_test, y_train, y_test = train_test_split(self.data['X'], self.data['Y'], test_size=0.1)
		self.model.compile(optimizer='adam', loss='mse', metrics=['accuracy'])
		self.model.fit(x_train, y_train, validation_data=(x_test, y_test), epochs=epochs)

	def save(self, path):
		pass

	def load(self, models):
		pass

	def predict(self, **kwargs):
		pass

class WageModel(BaseModel):
	def __init__ (self, data=None):
		self.data = data
		self.industryCode = data['industryCode']
		self.model = keras.Sequential([
				keras.layers.Dense(len(data['X'][0]), input_shape=(len(data['X'][0]),)),
				keras.layers.Dense(len(data['X'][0])+1, activation='relu'),
				keras.layers.Dense(1)
			])

	def load(self, model):
		pass

	def predict(self, income, lat, lon, years):
		timestamp = datetime.now.timestamp()
		predTimestamp = (datetime.now + timedelta(years=years)).timestamp()
		return self.model.predict([timestamp, predTimestamp, income, lat, lon])


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

	def train(self, epochs):
		savingsModel.train(epochs)
		cdModel.train(epochs)
		for s in stockModels:
			s.train(epochs)
		bondModel.train(epochs)
		tbModel.train(epochs)

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

	def train(self, epochs):
		residenceModel.train(epochs)
		rentModel.train(epochs)
		rmModel.train(epochs)

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

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class CDModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class StockModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class BondModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class TBModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class ResidenceModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		return self.model.predict([timestamp, predTimestamp, value, lat, lon])

class RentModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		return self.model.predict([timestamp, predTimestamp, value, lat, lon])

class RMModel(BaseModel):

	def load(self, model):
		pass

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])