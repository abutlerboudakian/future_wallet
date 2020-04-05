from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
from tensorflow import keras
from sklearn.model_selection import train_test_split
import numpy as np
import pandas as pd
from datasetbuilder import DatasetBuilder

from abc import ABC
from enum import Enum
from datetime import datetime, timedelta

class ModelType(Enum):
	WAGES = 1
	INVESTS = 2
	ASSETS = 3
	SAVINGS = 4
	CDS = 5
	STOCKS = 6
	BONDS = 7
	TBONDS = 8
	RES = 9
	RENTS = 10
	RMS = 11

class BaseModel(ABC):
	def __init__(self, dsb=None):
		if dsb is not None:
			self.dsb = dsb
			data = self.dsb.getData(1)
			self.model = keras.Sequential([
					keras.layers.Dense(len(data['X'].columns), input_shape=(len(data['X'].columns),)),
					keras.layers.Dense(len(data['X'].columns)+1, activation='relu'),
					keras.layers.Dense(1)
				])

	def setData(self, data):
		self.data = data

	def train(self, epochs, batchsize):
		for e in range(epochs):
			self.dsb.initQuery()
			while True:
				data = self.dsb.getData(batchsize)
				if not data:
					break
				x_train, x_test, y_train, y_test = train_test_split(self.data['X'], self.data['Y'], test_size=0.1)
				self.model.fit(x_train, y_train, validation_data=(x_test, y_test), batchsize=batchsize, epochs=e+1, initial_epoch=e)

	def save(self, path):
		pass

	def load(self, path):
		pass

	def predict(self, **kwargs):
		pass

class WageModel(BaseModel):
	def __init__ (self, industryCode=None):
		if industryCode is not None:
			self.industryCode = industryCode
			self.dsb = DatasetBuilder(ModelType.WAGES, industryCode=self.industryCode)
			data = self.dsb.getData(1)
			self.model = keras.Sequential([
					keras.layers.Dense(len(data['X'].columns), input_shape=(len(data['X'].columns),)),
					keras.layers.Dense(len(data['X'].columns)+1, activation='relu'),
					keras.layers.Dense(1)
				])
			self.model.compile(optimizer='adam', loss='mse', metrics=['accuracy'])


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
	def __init__(self, train=False, tickers=None):
		if train:
			self.savingsModel = SavingsModel(DatasetBuilder(ModelType.SAVINGS))
			self.cdModel = CDModel(DatasetBuilder(ModelType.CDS))
			self.stockModels = {}
			if tickers is None:
				tickers = dsb.getTickers()
			for ticker in tickers:
				self.stockModels[ticker] = StockModel(DatasetBuilder(ModelType.STOCKS, ticker=ticker))

			self.bondModel = BondModel(DatasetBuilder(ModelType.BONDS))
			self.tbModel = TBModel(DatasetBuilder(ModelType.TBONDS))
		else:
			self.savingsModel = SavingsModel()
			self.cdModel = CDModel()
			self.stockModels = {}
			self.bondModel = BondModel()
			self.tbModel = TBModel()

	def train(self, epochs, batchsize):
		savingsModel.train(epochs, batchsize)
		cdModel.train(epochs, batchsize)
		for s in stockModels:
			s.train(epochs, batchsize)
		bondModel.train(epochs, batchsize)
		tbModel.train(epochs, batchsize)

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
	def __init__(self, train=False):
		if train:
			self.residenceModel = ResidenceModel(DatasetBuilder(ModelType.RES))
			self.rentModel = RentModel(DatasetBuilder(ModelType.RENTS))
			self.rmModel = RMModel(DatasetBuilder(ModelType.RMS))
		else:
			self.residenceModel = ResidenceModel()
			self.rentModel = RentModel()
			self.rmModel = RMModel()

	def train(self, epochs, batchsize):
		residenceModel.train(epochs, batchsize)
		rentModel.train(epochs, batchsize)
		rmModel.train(epochs, batchsize)

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