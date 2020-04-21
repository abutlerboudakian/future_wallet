# Import statements
import sqlalchemy as sa
import pandas as pd
from datetime import datetime, timedelta
import dbutil
from enum import Enum
import modelpack

# Class Definition
class DatasetBuilder:

	'''
		@params:
			mType:		ModelType to generate dataset for
			batchsize:	Maximum number of rows to be fetched per batch for dataset
			**kwargs:
				industryCode:	Industry code to generate wages dataset for
				ticker:			Stock ticker to generate stock dataset for
		@requres:	mType is a valid ModelType, industryCode is specified if mType is WAGES, ticker is specified if mType is STOCKS
		@modifies:	self, engine, self.mType, self.batchsize, resultset, query
		@effects:	Instantiates a new DatasetBuilder object, initializes all instance fields, initializes database connection and pre-executes query
		@returns:	New DatasetBuilder instance
	'''
	def __init__(self, mType, batchsize=None, **kwargs):
		self.engine = dbutil.connect_engine('modeldata')
		self.mType = mType
		self.data = {}
		self.batchsize = batchsize
		self.resultset = None
		self.query = None
		if mType == modelpack.ModelType.WAGES:
			if 'industryCode' in kwargs:
				self.query = """SELECT
									c1.Timestamp AS Timestamp,
									c2.Timestamp AS PredTimestamp,
									c1.AnnualPayroll*1000/c1.NumEmployees AS Income,
									FIPS.Lat/90 AS Lat,
									FIPS.Long/180 AS Long,
									c2.AnnualPayroll*1000/c2.NumEmployees AS PredIncome
								FROM
									CBPIncome c1,
									CBPIncome c2,
									FIPS
								WHERE
									c1.StateCode = c2.StateCode AND
									c1.CountyCode = c2.CountyCode AND
									c1.IndustryCode = c2.IndustryCode AND
									c1.IndustryCode = '""" + kwargs['industryCode'] + """' AND
									c1.StateCode = FIPS.StateCode AND
									c1.CountyCode = FIPS.StateCode AND
									c2.Timestamp > c1.Timestamp AND
									c1.AnnualPayroll IS NOT NULL AND
									c2.AnnualPayroll IS NOT NULL AND
									c1.AnnualPayroll != 0 AND
									c2.AnnualPayroll != 0 AND
									c1.NumEmployees IS NOT NULL AND
									c2.NumEmployees IS NOT NULL AND
									c1.NumEmployees != 0 AND
									c2.NumEmployees != 0
									;"""
				

		elif mType == modelpack.ModelType.SAVINGS:
			self.query = """SELECT
								s1.Timestamp AS Timestamp,
								s2.Timestamp AS PredTimestamp,
								s1.NationalRate AS Rate,
								s2.NationalRate AS PredRate
							FROM
								NJSAs s1,
								NJSAs s2
							WHERE
								s1.Timestamp < s2.Timestamp AND
								s1.NationalRate IS NOT NULL AND
								s2.NationalRate IS NOT NULL
							UNION ALL
							SELECT
								s1.Timestamp AS Timestamp,
								s2.Timestamp AS PredTimestamp,
								s1.NationalRate AS Rate,
								s2.NationalRate AS PredRate
							FROM
								JSAs s1,
								JSAs s2
							WHERE
								s1.Timestamp < s2.Timestamp AND
								s1.NationalRate IS NOT NULL AND
								s2.NationalRate IS NOT NULL
							;"""

		elif mType == modelpack.ModelType.CDS:
			self.query = """SELECT
							c1.Timestamp AS Timestamp,
							c2.Timestamp AS PredTimestamp,
							c1.Rate AS Rate,
							c2.Rate AS PredRate
						FROM
							CDs c1,
							CDs c2
						WHERE
							c1.Timestamp < c2.Timestamp AND
							c1.Rate IS NOT NULL AND
							c2.Rate IS NOT NULL
						;"""

		elif mType == modelpack.ModelType.STOCKS:
			if 'ticker' in kwargs:
				t = kwargs['ticker']
				self.query = """SELECT
								s1.Timestamp AS Timestamp,
								s2.Timestamp AS PredTimestamp,
								s1.[Close] AS Price,
								s2.[Close] AS PredPrice
							FROM
								Stocks s1,
								Stocks s2
							WHERE
								s1.Ticker = '""" + t + """' AND
								s1.Ticker = s2.Ticker AND
								s1.Timestamp < s2.Timestamp AND
								s1.[Close] IS NOT NULL AND
								s2.[Close] IS NOT NULL
							;"""

		elif mType == modelpack.ModelType.BONDS:
			self.query = """SELECT
							b1.Timestamp AS Timestamp,
							b2.Timestamp AS PredTimestamp,
							b1.Rate AS Rate,
							b2.Rate AS PredRate
						FROM
							Bonds b1,
							Bonds b2
						WHERE
							b1.Timestamp > b2.Timestamp AND
							b1.Rate IS NOT NULL AND
							b2.Rate IS NOT NULL
						;"""


		elif mType == modelpack.ModelType.TBONDS:
			self.query = """SELECT
							b1.Timestamp AS Timestamp,
							b2.Timestamp AS PredTimestamp,
							b1.PrimeRate AS Rate,
							b2.PrimeRate AS PredRate
						FROM
							TBonds b1,
							TBonds b2
						WHERE
							b1.Timestamp > b2.Timestamp AND
							b1.PrimeRate IS NOT NULL AND
							b2.PrimeRate IS NOT NULL
						;"""


		elif mType == modelpack.ModelType.RES:
			self.query = """SELECT
							h1.Timestamp AS Timestamp,
							h2.Timestamp AS PredTimestamp,
							h1.[Index] AS Price,
							FIPS.Lat/90 AS Lat,
							FIPS.Long/180 AS Long,
							h2.[Index] AS PredPrice
						FROM
							HI h1,
							HI h2,
							FIPS
						WHERE
							h1.ZIP = h2.ZIP AND
							h1.ZIP = FIPS.ZIP AND
							h1.Timestamp < h2.Timestamp AND
							h1.[Index] IS NOT NULL AND
							h2.[Index] IS NOT NULL
						;"""


		elif mType == modelpack.ModelType.RENTS:
			self.query = """SELECT TOP(10000000)
							r1.Timestamp AS Timestamp,
							r2.Timestamp AS PredTimestamp,
							r1.Rent AS Rent,
							FIPS.Lat/90 AS Lat,
							FIPS.Long/180 AS Long,
							r2.Rent AS PredRent
						FROM
							ZillowRent r1,
							ZillowRent r2,
							FIPS
						WHERE
							r1.ZIPCode = r2.ZIPCode AND
							r1.ZIPCode = FIPS.ZIP AND
							r1.Timestamp < r2.Timestamp AND
							r1.Rent IS NOT NULL AND
							r2.Rent IS NOT NULL
						;"""

		elif mType == modelpack.ModelType.RMS:
			self.query = """SELECT
							r1.Timestamp AS Timestamp,
							r2.Timestamp AS PredTimestamp,
							r1.Price AS Price,
							r2.Price AS PredPrice
						FROM
							RMI r1,
							RMI r2
						WHERE
							r1.Timestamp < r2.Timestamp AND
							r1.Price IS NOT NULL AND
							r2.Price IS NOT NULL
						;"""
		else:
			raise ValueError('Invalid model type specified. ' + mType)

		if self.query is not None:
			self.resultset = self.engine.execute(self.query)
		else:
			raise ValueError('Missing argument for model specified')

	'''
		@params:	batchsize - Custom size to override instance batchsize
		@requires:	Object is already instantiated, valid query pre-loaded
		@modifies:	resultset
		@effects:	Fetches [batchsize] rows from resultset, preprocesses data for training
		@returns:	Pandas dataframe containing batch of training data
	'''
	def getData(self, batchsize=None):
		if batchsize is not None:
			data = self.resultset.fetchmany(batchsize)
		else:
			data = self.resultset.fetchmany(self.batchsize)
		df = pd.DataFrame(data, columns=self.resultset.keys())
		df['Timestamp'] = df['Timestamp'].astype('int64')
		df['PredTimestamp'] = df['PredTimestamp'].astype('int64')
		df['DeltaTime'] = 1000*((df['PredTimestamp'] - df['Timestamp']) / df['PredTimestamp'])
		
		ret = {}
		if self.mType == modelpack.ModelType.WAGES:
			ret['X'] = df[['DeltaTime', 'Income', 'Lat', 'Long']]
			ret['Y'] = df[['PredIncome']]
		elif self.mType == modelpack.ModelType.SAVINGS:
			ret['X'] = df[['DeltaTime', 'Rate']]
			ret['Y'] = df[['PredRate']]
		elif self.mType == modelpack.ModelType.CDS:
			ret['X'] = df[['DeltaTime', 'Rate']]
			ret['Y'] = df[['PredRate']]
		elif self.mType == modelpack.ModelType.STOCKS:
			ret['X'] = df[['DeltaTime', 'Price']]
			ret['Y'] = df[['PredPrice']]
		elif self.mType == modelpack.ModelType.BONDS:
			ret['X'] = df[['DeltaTime', 'Rate']]
			ret['Y'] = df[['PredRate']]
		elif self.mType == modelpack.ModelType.TBONDS:
			ret['X'] = df[['DeltaTime', 'Rate']]
			ret['Y'] = df[['PredRate']]
		elif self.mType == modelpack.ModelType.RES:
			ret['X'] = df[['DeltaTime', 'Price', 'Lat', 'Long']]
			ret['Y'] = df[['PredPrice']]
		elif self.mType == modelpack.ModelType.RENTS:
			ret['X'] = df[['DeltaTime', 'Rent', 'Lat', 'Long']]
			ret['Y'] = df[['PredRent']]
		elif self.mType == modelpack.ModelType.RMS:
			ret['means'] = (df['Timestamp'].mean(), df['PredTimestamp'].mean())
			ret['stds'] = (df['Timestamp'].std(), df['PredTimestamp'].std())
			df['Timestamp'] = (df['Timestamp'] - df['Timestamp'].mean()) / df['Timestamp'].std()
			df['PredTimestamp'] = (df['PredTimestamp'] - df['PredTimestamp'].mean()) / df['PredTimestamp'].std()
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Price']]
			ret['Y'] = df[['PredPrice']]
		return ret

	'''
		@requires:	Instance query is valid
		@modifies:	resultset
		@effects:	Initializes database query and stores it into resultset
		@returns:	None
	'''
	def initQuery(self):
		self.resultset = self.engine.execute(self.query)