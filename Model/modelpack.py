# Import statements
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

# Enum definition for all model types
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

# Class definition for abstract model
class BaseModel(ABC):
    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new BaseModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new BaseModel instance
    '''
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

    '''
        @params:
            epochs -    Number of times to loop through the entire dataset
            batchsize - Size of each batch of data being loaded in through memory
        @requires:  dsb is initialized with a valid modeltype
        @modifies:  dsb, model
        @effects:   Loads in data from dsb one batch at a time and trains the model using that data
        @returns:   None
    '''
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
                xTrain, xTest, yTrain, yTest = train_test_split(data['X'], data['Y'], test_size=0.1)
                self.model.fit(xTrain, yTrain, validation_data=(xTest, yTest), batch_size=batchsize, epochs=e+1, initial_epoch=e)
                if not small:
                    data = self.dsb.getData(batchsize)
                else:
                    break
    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        self.model.save(path)

    ''' Method declaration for overriding
        @params:    path - Directory to load the model from
    '''
    def load(self, path):
        pass

    ''' Method declaration for overriding
    '''
    def predict(self, **kwargs):
        pass

# Class definition for wage model
class WageModel(BaseModel):

    '''
        @params:    industryCode - Industry code this model is affiliated with
        @modifies:  self, self.industryCode, self.dsb, model
        @effects:   Instantiates a new WageModel object, generates an industry specific neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new WageModel instance
    '''
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

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        if self.industryCode.endswith('/'):
            self.model.save(path + 'wagemodel' + self.industryCode)
        else:
            self.model.save(path + 'wagemodel' + self.industryCode + '/')

    '''
        @params:
            path -          Directory to load the model from
            industryCode -  Industry to load the model from
        @requires:  model exists in storage at path with industryCode specified
        @modifies:  industryCode, model
        @effects:   Loads internal model from disk from specified path with specified industry code
        @returns:   None
    '''
    def load(self, path, industryCode):
        self.industryCode = industryCode
        loader_impl.parse_saved_model(path + 'wagemodel' + industryCode)
        self.model = saved_model_load.load(path + 'wagemodel' + industryCode, True)

    '''
        @params:
            income -    Current income
            lat -       Latitude of occupation
            lon -       Longitude of occupation
            years -     Years into future to predict
        @requires:  model is trained and loaded
        @modifies:  None
        @effects:   Takes parameters, preprocesses them and feeds them into neural network to get wage prediction
        @returns:   Prediction value for income
    '''
    def predict(self, income, lat, lon, years):
        timestamp = datetime.now().timestamp()
        today = datetime.now()
        predTimestamp = (today.replace(year=today.year + years)).timestamp()
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, income, lat/90, lon/180]]))[0][0])
        return prediction - income if prediction > 0 else 0

# Class definition for Investment Model
class InvestmentModel(BaseModel):

    '''
        @params:
            train -     flag for if model is being instantiated for training or predicting
            tickers -   List of tickers to train stock models for
        @modifies:  self, savingsModel, cdModel, stockModels, stockModels, bondModel, tbModel
        @effects:   Instantiates a new InvestmentModel object, generates a neural network for each internal model if train is enabled
        @returns:   New InvestmentModel instance
    '''
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

    '''
        @params:
            epochs -    Number of times to loop through the entire dataset
            batchsize - Size of each batch of data being loaded in through memory
        @requires:  All internal models are initialized
        @modifies:  savingsModel, cdModel, stockModels, bondModel, tbModel
        @effects:   Delegates training to each internal model separately
        @returns:   None
    '''
    def train(self, epochs, batchsize):
        self.savingsModel.train(epochs, batchsize)
        self.cdModel.train(epochs, batchsize)
        for ticker, model in self.stockModels.items():
            model.train(epochs, batchsize)
        self.bondModel.train(epochs, batchsize)
        self.tbModel.train(epochs, batchsize)

    '''
        @params:    path - Directory to save the model to
        @requires:  All models are instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        self.savingsModel.save(path)
        self.cdModel.save(path)
        for ticker, model in self.stockModels.items():
            model.save(path, ticker)
        self.bondModel.save(path)
        self.tbModel.save(path)

    '''
        @params:
            path -      Directory to load the model from
            tickers -   Tickers to load stock models for
        @requires:  All internal models are stored at location path, with all stock models that match tickers existing
        @modifies:  savingsModel, cdModel, stockModels, bondModel, tbModel
        @effects:   Loads internal models from disk from specified path, with stock models being loaded from tickers specified
        @returns: None
    '''
    def load(self, path, tickers):
        self.savingsModel.load(path)
        self.cdModel.load(path)
        for t in tickers:
            self.stockModels[t] = StockModel()
            self.stockModels[t].load(path, t)
        self.bondModel.load(path)
        self.tbModel.load(path)

    '''
        @params:
            savings -   Amount in all savings accounts
            cd -        Amount in all CD accounts
            stocks -    List of tuples containing the stock ticker, current price, and number of shares
            bonds -     Amount owned in bonds
            tbonds -    Amount owned in Treasury bonds
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
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
        
# Class definition for AssetModel
class AssetModel(BaseModel):

    '''
        @params:
            train -     flag for if model is being instantiated for training or predicting
        @modifies:  self, residenceModel, rentModel, rmModel
        @effects:   Instantiates a new AssetModel object, generates a neural network for each internal model if train is enabled
        @returns:   New AssetModel instance
    '''
    def __init__(self, train=False):
        if train:
            self.residenceModel = ResidenceModel(DatasetBuilder(ModelType.RES))
            self.rentModel = RentModel(DatasetBuilder(ModelType.RENTS))
            self.rmModel = RMModel(DatasetBuilder(ModelType.RMS))
        else:
            self.residenceModel = ResidenceModel()
            self.rentModel = RentModel()
            self.rmModel = RMModel()

    '''
        @params:
            epochs -    Number of times to loop through the entire dataset
            batchsize - Size of each batch of data being loaded in through memory
        @requires:  All internal models are initialized
        @modifies:  residenceModel, rentModel, rmModel
        @effects:   Delegates training to each internal model separately
        @returns:   None
    '''
    def train(self, epochs, batchsize):
        self.residenceModel.train(epochs, batchsize)
        self.rentModel.train(epochs, batchsize)
        self.rmModel.train(epochs, batchsize)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        self.residenceModel.save(path)
        self.rentModel.save(path)
        self.rmModel.save(path)

    '''
        @params:
            path -      Directory to load the model from
        @requires:  All internal models are stored at location path
        @modifies:  residenceModel, rentModel, rmModel
        @effects:   Loads internal models from disk from specified path, with stock models being loaded from tickers specified
        @returns: None
    '''
    def load(self, path):
        self.residenceModel.load(path)
        self.rentModel.load(path)
        self.rmModel.load(path)

    '''
        @params:
            residences -    List of tuples containing the residence price, latitude, and longitude
            rents -         List of tuples containing the rent amount, latitude, and longitude
            rm -            Amount owned in rare-metal assets
            years -         Number of years to predict in the future
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
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

# Class definition for savings model
class SavingsModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new SavingsModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new SavingsModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'savingsmodel/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'savingsmodel')
        self.model = saved_model_load.load(path + 'savingsmodel', True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Amount in savings account
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])
        return prediction if prediction > 0 else 0

class CDModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new CDModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new CDModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'cdmodel/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'cdmodel')
        self.model = saved_model_load.load(path + 'cdmodel', True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Amount in CD account
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])
        return prediction if prediction > 0 else 0

class StockModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new StockModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new StockModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path, ticker):
        super().save(path + 'stockmodel' + ticker + '/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path, ticker):
        loader_impl.parse_saved_model(path + 'stockmodel' + ticker)
        self.model = saved_model_load.load(path + 'stockmodel' + ticker, True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Current stock price
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])
        return prediction if prediction > 0 else 0

class BondModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new BaseModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new BaseModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'bondmodel/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'bondmodel')
        self.model = saved_model_load.load(path + 'bondmodel', True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Amount in bonds
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])
        return prediction if prediction > 0 else 0

class TBModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new TBModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new TBModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'tbmodel/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'tbmodel')
        self.model = saved_model_load.load(path + 'tbmodel', True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Amount in treasury bonds
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value]]))[0][0])
        return prediction if prediction > 0 else 0

class ResidenceModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new ResidenceModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new ResidenceModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'resmodel/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'resmodel')
        self.model = saved_model_load.load(path + 'resmodel', True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Home value
            lat -           Latitude
            lon -           Longitude
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value, lat, lon):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value, lat/90, lon/180]]))[0][0])
        return prediction if prediction > 0 else 0

class RentModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new RentModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new RentModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'rentmodel/')

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'rentmodel')
        self.model = saved_model_load.load(path + 'rentmodel', True)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Rent value
            lat -           Latitude
            lon -           Longitude
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value, lat, lon):
        deltaTime = 1000*(predTimestamp - timestamp) / predTimestamp
        prediction = float(self.model.predict(np.asarray([[deltaTime, value, lat/90, lon/180]]))[0][0])
        return prediction if prediction > 0 else 0

class RMModel(BaseModel):

    '''
        @params:    dsb - DatasetBuilder instance for training dataset
        @modifies:  self, self.dsb, model
        @effects:   Instantiates a new RMModel object, generates a neural network based off of the inputs provided in dsb
            and compiles the neural network
        @returns new RMModel instance
    '''
    def __init__(self, dsb=None):
        super().__init__(dsb)
        self.mean = None
        self.std = None

    '''
        @params:
            epochs -    Number of times to loop through the entire dataset
            batchsize - Size of each batch of data being loaded in through memory
        @requires:  dsb is initialized with a valid modeltype
        @modifies:  dsb, model
        @effects:   Loads in data from dsb one batch at a time and trains the model using that data
        @returns:   None
    '''
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
                xTrain, xTest, yTrain, yTest = train_test_split(data['X'], data['Y'], test_size=0.1)
                self.model.fit(xTrain, yTrain, validation_data=(xTest, yTest), batch_size=batchsize, epochs=e+1, initial_epoch=e)
                if not small:
                    data = self.dsb.getData(batchsize)
                    means.append(data['means'])
                    stds.append(data['stds'])
                else:
                    break
        self.mean = [sum(i)/len(i) for i in zip(*means)]  if len(means) > 0 else 0
        self.std = [sum(i)/len(i) for i in zip(*stds)] if len(stds) > 0 else 0

    '''
        @params:    path - Directory to save the model to
        @requires:  model is instantiated
        @modifies:  Storage
        @effects:   Saves internal model to disk at specified path
        @returns:   None
    '''
    def save(self, path):
        super().save(path + 'rmmodel/')
        with open(path + 'rmmodel/mean', 'wb') as outfile:
            pickle.dump(self.mean, outfile)
        with open(path + 'rmmodel/std', 'wb') as outfile:
            pickle.dump(self.std, outfile)

    '''
        @params:    path - Directory to load the model from
        @requires:  model is stored at directory [path]
        @modifies:  model
        @effects:   Loads the internal model from disk
        @returns: None
    '''
    def load(self, path):
        loader_impl.parse_saved_model(path + 'rmmodel')
        self.model = saved_model_load.load(path + 'rmmodel', True)
        with open(path + 'rmmodel/mean', 'rb') as infile:
            self.mean = pickle.load(infile)
        with open(path + 'rmmodel/std', 'rb') as outfile:
            self.std = pickle.load(outfile)

    '''
        @params:
            timestamp -     Starting time in epoch format
            predTimestamp - Ending time in epoch format
            value -         Amount in rare metals owned
        @requires:  All internal models are trained and loaded
        @modifies:  None
        @effects:   Takes all paramters, preprocesses them and feeds them into their respective models,
            then formats the output and returns the prediction
    '''
    def predict(self, timestamp, predTimestamp, value):
        timestamp = (timestamp - self.mean[0]) / self.std[0]
        predTimestamp = (predTimestamp - self.mean[1]) / self.std[1]
        prediction = float(self.model.predict(np.asarray([[timestamp, predTimestamp, value]]))[0][0])
        return prediction if prediction > 0 else 0
