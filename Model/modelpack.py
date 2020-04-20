from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
from tensorflow import keras
from tensorflow.python.keras.saving.saved_model import load as saved_model_load
from tensorflow.python.saved_model import loader_impl
from sklearn.model_selection import train_test_split
import numpy as np
import pandas as pd
import pickle
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
		loader_impl.parse_saved_model(path + 'wagemodel' + industryCode)
		self.model = saved_model_load.load(path + 'wagemodel' + industryCode, True)

	def predict(self, income, lat, lon, years):
		timestamp = datetime.now().timestamp()
		today = datetime.now()
		predTimestamp = (today.replace(year=today.year + years)).timestamp()
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, income, lat/90, lon/180]]))[0][0])


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
			self.stockModels[t] = StockModel()
			self.stockModels[t].load(path, t)
		self.bondModel.load(path)
		self.tbModel.load(path)


	def predict(self, savings, cd, stocks, bonds, tbs, years):
		stockRet = 0
		timestamp = datetime.now().timestamp()
		today = datetime.now()
		predTimestamp = (today.replace(year=today.year + years)).timestamp()
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
		#self.residenceModel.train(epochs, batchsize)
		#self.rentModel.train(epochs, batchsize)
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
		today = datetime.now()
		predTimestamp = (today.replace(year=today.year + years)).timestamp()
		for (res, lat, lon) in residences:
			predRes = self.residenceModel.predict(timestamp, predTimestamp, res, lat, lon)
			resRet += (predRes - res)/years
		for (rent, lat, lon) in rents:
			predRent = self.rentModel.predict(timestamp, predTimestamp, rent, lat, lon)
			rentRet += predRent*12
		predRm = self.rmModel.predict(timestamp, predTimestamp, rm)
		return resRet + rentRet + predRm

class SavingsModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'savingsmodel/')

	def load(self, path):
		loader_impl.parse_saved_model(path + 'savingsmodel')
		self.model = saved_model_load.load(path + 'savingsmodel', True)

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])

class CDModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'cdmodel/')

	def load(self, path):
		loader_impl.parse_saved_model(path + 'cdmodel')
		self.model = saved_model_load.load(path + 'cdmodel', True)

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])

class StockModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path, ticker):
		super().save(path + 'stockmodel' + ticker + '/')

	def load(self, path, ticker):
		loader_impl.parse_saved_model(path + 'stockmodel' + ticker)
		self.model = saved_model_load.load(path + 'stockmodel' + ticker, True)

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])

class BondModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'bondmodel/')

	def load(self, path):
		loader_impl.parse_saved_model(path + 'bondmodel')
		self.model = saved_model_load.load(path + 'bondmodel', True)

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])

class TBModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'tbmodel/')

	def load(self, path):
		loader_impl.parse_saved_model(path + 'tbmodel')
		self.model = saved_model_load.load(path + 'tbmodel', True)

	def predict(self, timestamp, predTimestamp, value):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])

class ResidenceModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'resmodel/')

	def load(self, path):
		loader_impl.parse_saved_model(path + 'resmodel')
		self.model = saved_model_load.load(path + 'resmodel', True)

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value, lat/90, lon/180]]))[0][0])

class RentModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)

	def save(self, path):
		super().save(path + 'rentmodel/')

	def load(self, path):
		loader_impl.parse_saved_model(path + 'rentmodel')
		self.model = saved_model_load.load(path + 'rentmodel', True)

	def predict(self, timestamp, predTimestamp, value, lat, lon):
		deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
		return float(self.model.predict(np.asarray([[deltaTime, value, lat/90, lon/180]]))[0][0])

class RMModel(BaseModel):
	def __init__(self, dsb=None):
		super().__init__(dsb)
		self.mean = None
		self.std = None

	def train(self, epochs, batchsize):
		small = False
		self.dsb.initQuery()
		data = self.dsb.getData(batchsize)
		means = [data['means']]
		stds = [data['stds']]
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
					means.append(data['means'])
					stds.append(data['stds'])
				else:
					break
		self.mean = [sum(i)/len(i) for i in zip(*means)]  if len(means) > 0 else 0
		self.std = [sum(i)/len(i) for i in zip(*stds)] if len(stds) > 0 else 0

	def save(self, path):
		super().save(path + 'rmmodel/')
		with open(path + 'rmmodel/mean', 'wb') as outfile:
			pickle.dump(self.mean, outfile)
		with open(path + 'rmmodel/std', 'wb') as outfile:
			pickle.dump(self.std, outfile)

	def load(self, path):
		loader_impl.parse_saved_model(path + 'rmmodel')
		self.model = saved_model_load.load(path + 'rmmodel', True)
		with open(path + 'rmmodel/mean', 'rb') as infile:
			self.mean = pickle.load(infile)
		with open(path + 'rmmodel/std', 'rb') as outfile:
			self.std = pickle.load(outfile)

	def predict(self, timestamp, predTimestamp, value):
		timestamp = (timestamp - self.mean[0]) / self.std[0]
		predTimestamp = (predTimestamp - self.mean[1]) / self.std[1]
		return float(self.model.predict(np.asarray([[timestamp, predTimestamp, value]]))[0][0])
