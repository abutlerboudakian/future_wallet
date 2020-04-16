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
					keras.layers.Dense((len(data['X'].columns)*1.5) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*2) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*5) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*10) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense(1)
				])
			self.model.compile(optimizer='adam', loss=keras.losses.mean_absolute_percentage_error, metrics=['mean_absolute_percentage_error'])

	def setData(self, data):
		self.data = data

	def train(self, epochs, batchsize):
		small = False
		self.dsb.initQuery()
		data = self.dsb.getData(batchsize)
		if len(data['X'].index) < batchsize:
			small = True
		for e in range(epochs):
			if not small:
				self.dsb.initQuery()
				data = self.dsb.getData(batchsize)
			while True:		
				if data['X'].empty or data['Y'].empty:
					break
				x_train, x_test, y_train, y_test = train_test_split(data['X'], data['Y'], test_size=0.1)
				self.model.fit(x_train, y_train, validation_data=(x_test, y_test), batch_size=batchsize, epochs=e+1, initial_epoch=e)
				if not small:
					data = self.dsb.getData(batchsize)
				else:
					break

	def save(self, path):
		self.model.save(path)

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
					keras.layers.Dense((len(data['X'].columns)*1.5) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*2) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*5) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*10) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*10) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*5) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense((len(data['X'].columns)*2) // 1),
					keras.layers.LeakyReLU(),
					keras.layers.Dense(1)
				])
			self.model.compile(optimizer='adam', loss=keras.losses.mean_absolute_percentage_error, metrics=['mean_absolute_percentage_error'])


	def save(self, path):
		if self.industryCode.endswith('/'):
			self.model.save(path + 'wagemodel' + self.industryCode)
		else:
			self.model.save(path + 'wagemodel' + self.industryCode + '/')

	def load(self, path, industryCode):
		self.industryCode = industryCode
		self.model = keras.models.load_model(path + 'wagemodel' + industryCode)

	def predict(self, income, lat, lon, years):
		timestamp = datetime.now().timestamp()
		today = datetime.now()
		predTimestamp = (today.replace(year=today.year + years)).timestamp()
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict(np.asarray([[deltaTime, income, lat/90, lon/180]]))


class InvestmentModel(BaseModel):
	def __init__(self, train=False, tickers=None):
		if train:
			self.savingsModel = SavingsModel(DatasetBuilder(ModelType.SAVINGS))
			self.cdModel = CDModel(DatasetBuilder(ModelType.CDS))
			self.stockModels = {}
			if tickers is not None:
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
		self.savingsModel.train(epochs, batchsize)
		self.cdModel.train(epochs, batchsize)
		for ticker, model in self.stockModels.items():
			model.train(epochs, batchsize)
		self.bondModel.train(epochs, batchsize)
		self.tbModel.train(epochs, batchsize)

	def save(self, path):
		self.savingsModel.save(path)
		self.cdModel.save(path)
		for ticker, model in self.stockModels.items():
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
			predStock = self.stockModels[ticker].predict(timestamp, predTimestamp, stock)
			stockRet += ((predStock - stock)*shares)
		stockRet /= years
		return stockRet + self.savingsModel.predict(timestamp, predTimestamp, savings) + self.cdModel.predict(timestamp, predTimestamp, cd) + self.bondModel.predict(timestamp, predTimestamp, bonds) + self.tbModel.predict(timestamp, predTimestamp, tbs)
		

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
		self.residenceModel.train(epochs, batchsize)
		self.rentModel.train(epochs, batchsize)
		self.rmModel.train(epochs, batchsize)

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
			predRes = self.residenceModel.predict(timestamp, predTimestamp, res, lat, lon)
			resRet += (predRes - res)/years
		for (rent, lat, lon) in rents:
			predRent = self.rentModel.predict(timestamp, predTimestamp, rent, lat, lon)
			rentRet += predRent*12
		return resRet + rentRet + self.rmModel.predict(timestamp, predTimestamp, rm)

class SavingsModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'savingsmodel/')

	def load(self, path):
		self.model = keras.models.load_model(path + 'savingsmodel')

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value])

class CDModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'cdmodel/')

	def load(self, path):
		self.model = keras.models.load_model(path + 'cdmodel')

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value])

class StockModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path, ticker):
		super().save(path + 'stockmodel' + ticker + '/')

	def load(self, model):
		self.model = keras.models.load_model(path + 'stockmodel' + ticker)

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value])

class BondModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'bondmodel/')

	def load(self, path):
		self.model = keras.models.load_model(path + 'bondmodel')

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value])

class TBModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'tbmodel/')

	def load(self, path):
		self.model = keras.models.load_model(path + 'tbmodel')

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value])

class ResidenceModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'resmodel/')

	def load(self, path):
		self.model = keras.models.load_model(path + 'resmodel')

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value, lat/90, lon/180])

class RentModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'rentmodel/')

	def load(self, path):
		self.model = keras.models.load_model(path + 'rentmodel')

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value, lat/90, lon/180])

class RMModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'rmmodel/')

	def load(self, model):
		self.model = keras.models.load_model(path + 'rmmodel')

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return self.model.predict([deltaTime, value])