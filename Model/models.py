from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
from tensorflow import keras
from sklearn.model_selection import train_test_split
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
		if data is not None:
			self.data = data
			self.model = keras.Sequential([
					keras.layers.Dense(len(data['X'].columns), input_shape=(len(data['X'].columns),)),
					keras.layers.Dense(len(data['X'].columns)+1, activation='relu'),
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

	def load(self, path):
		pass

	def predict(self, **kwargs):
		pass

class WageModel(BaseModel):
	def __init__ (self, data=None):
		if data is not None:
			self.data = data
			self.industryCode = data['industryCode']
			self.model = keras.Sequential([
					keras.layers.Dense(len(data['X'].columns), input_shape=(len(data['X'].columns),)),
					keras.layers.Dense(len(data['X'].columns)+1, activation='relu'),
					keras.layers.Dense(1)
				])


	def save(self, path):
		self.model.save(path + 'wagemodel' + self.industryCode)

	def load(self, path, industryCode):
		self.industryCode = industryCode
		self.model = keras.models.load_model(path + 'wagemodel' + industryCode)

	def predict(self, income, lat, lon, years):
		timestamp = datetime.now.timestamp()
		predTimestamp = (datetime.now + timedelta(years=years)).timestamp()
		return self.model.predict([timestamp, predTimestamp, income, lat, lon])


class InvestmentModel(BaseModel):
	def __init__(self, data=None):
		if data is not None:
			savingsData, cdData, stockData, bondData, tbData = zip(*data)
			self.savingsModel = SavingsModel(savingsData)
			self.cdModel = CDModel(savingsData)
			self.stockModels = {}
			for ticker, sData in stockData.items():
				self.stockModels[ticker] = StockModel(sData)
			self.bondModel = BondModel(bondData)
			self.tbModel = TBModel(tbData)
		else:
			self.savingsModel = SavingsModel()
			self.cdModel = CDModel()
			self.stockModels = {}
			self.bondModel = BondModel()
			self.tbModel = TBModel()

	def train(self, epochs):
		savingsModel.train(epochs)
		cdModel.train(epochs)
		for s in stockModels:
			s.train(epochs)
		bondModel.train(epochs)
		tbModel.train(epochs)

	def save(self, path):
		self.savingsModel.save(path)
		self.cdModel.save(path)
		for ticker, model in self.stockModels:
			model.save(path, ticker)
		self.bondModel.save(path)
		self.tbModel.save(path)

	def load(self, path, tickers):
		self.savingsModel.load(path)
		self.cdModel.load(path)
		for t in tickers:
			stockModels[t] = StockModel()
			stockModels[t].load(path, t)
		self.bondModel.load(path)
		self.tbModel.load(path)


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
		if data is not None:
			resData, rentData, rmData = zip(*data)
			self.residenceModel = ResidenceModel(resData)
			self.rentModel = RentModel(rentData)
			self.rmModel = RMModel(rmData)
		else:
			self.residenceModel = ResidenceModel()
			self.rentModel = RentModel()
			self.rmModel = RMModel()

	def train(self, epochs):
		residenceModel.train(epochs)
		rentModel.train(epochs)
		rmModel.train(epochs)

	def save(self, path):
		self.residenceModel.save(path)
		self.rentModel.save(path)
		self.rmModel.save(path)

	def load(self, path):
		self.residenceModel.load(path)
		self.rentModel.load(path)
		self.rmModel.load(path)

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

	def save(self, path):
		self.model.save(path + 'savingsmodel')

	def load(self, path):
		self.model = keras.models.load_model(path + 'savingsmodel')

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class CDModel(BaseModel):
	def save(self, path):
		self.model.save(path + 'cdmodel')

	def load(self, path):
		self.model = keras.models.load_model(path + 'cdmodel')

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class StockModel(BaseModel):

	def save(self, path, ticker):
		self.model.save(path + 'stockmodel' + ticker)

	def load(self, model):
		self.model = keras.models.load_model(path + 'stockmodel' + ticker)

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class BondModel(BaseModel):

	def save(self, path):
		self.model.save(path + 'bondmodel')

	def load(self, path):
		self.model = keras.models.load_model(path + 'bondmodel')

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class TBModel(BaseModel):

	def save(self, path):
		self.model.save(path + 'tbmodel')

	def load(self, path):
		self.model = keras.models.load_model(path + 'tbmodel')

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])

class ResidenceModel(BaseModel):

	def save(self, path):
		self.model.save(path + 'resmodel')

	def load(self, path):
		self.model = keras.models.load_model(path + 'resmodel')

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		return self.model.predict([timestamp, predTimestamp, value, lat, lon])

class RentModel(BaseModel):

	def save(self, path):
		self.model.save(path + 'rentmodel')

	def load(self, path):
		self.model = keras.models.load_model(path + 'rentmodel')

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		return self.model.predict([timestamp, predTimestamp, value, lat, lon])

class RMModel(BaseModel):

	def save(self, path):
		self.model.save(path + 'rmmodel')

	def load(self, model):
		self.model = keras.models.load_model(path + 'rmmodel')

	def predict(self, timestamp, predTimestamp, value):
		return self.model.predict([timestamp, predTimestamp, value])