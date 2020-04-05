import sqlalchemy as sa
import pandas as pd
from datetime import datetime, timedelta
import dbutil
from enum import Enum
import modelpack

class DatasetBuilder:
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
									c1.AnnualPayroll AS Income,
									FIPS.Lat AS Lat,
									FIPS.Long AS Long,
									c2.AnnualPayroll AS PredIncome
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
									c2.Timestamp > c1.Timestamp
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
								s1.Timestamp < s2.Timestamp
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
								s1.Timestamp < s2.Timestamp
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
							c1.Timestamp < c2.Timestamp
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
								s1.Timestamp < s2.Timestamp
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
							b1.Timestamp > b2.Timestamp
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
							b1.Timestamp > b2.Timestamp
						;"""


		elif mType == modelpack.ModelType.RES:
			self.query = """SELECT
							h1.Timestamp AS Timestamp,
							h2.Timestamp AS PredTimestamp,
							h1.[Index] AS Price,
							FIPS.Lat AS Lat,
							FIPS.Long AS Long,
							h2.[Index] AS PredPrice
						FROM
							HI h1,
							HI h2,
							FIPS
						WHERE
							h1.ZIP = h2.ZIP AND
							h1.ZIP = FIPS.ZIP AND
							h1.Timestamp < h2.Timestamp
						;"""


		elif mType == modelpack.ModelType.RENTS:
			self.query = """SELECT
							r1.Timestamp AS Timestamp,
							r2.Timestamp AS PredTimestamp,
							r1.Rent AS Rent,
							FIPS.Lat AS Lat,
							FIPS.Long AS Long,
							r2.Rent AS PredRent
						FROM
							ZillowRent r1,
							ZillowRent r2,
							FIPS
						WHERE
							r1.ZIPCode = r1.ZIPCode AND
							r1.ZIPCode = FIPS.ZIP AND
							r1.Timestamp < r2.Timestamp
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
							r1.Timestamp < r2.Timestamp
						;"""
		else:
			raise 'Invalid model type specified. ' + mType

		if self.query is not None:
			self.resultset = self.engine.execute(self.query)
		else:
			raise 'Missing argument for model specified'

	def getData(self, batchsize=None):
		if batchsize is not None:
			data = self.resultset.fetchmany(batchsize)
		else:
			data = self.resultset.fetchmany(self.batchsize)
		df = pd.DataFrame(data, columns=self.resultset.keys())
		ret = {}
		if self.mType == modelpack.ModelType.WAGES:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Income', 'Lat', 'Long']]
			ret['Y'] = df[['PredIncome']]
		if self.mType == modelpack.ModelType.SAVINGS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Rate']]
			ret['Y'] = df[['PredRate']]
		if self.mType == modelpack.ModelType.CDS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Rate']]
			ret['Y'] = df[['PredRate']]
		if self.mType == modelpack.ModelType.STOCKS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Price']]
			ret['Y'] = df[['PredPrice']]
		if self.mType == modelpack.ModelType.BONDS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Rate']]
			ret['Y'] = df[['PredRate']]
		if self.mType == modelpack.ModelType.TBONDS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Rate']]
			ret['Y'] = df[['PredRate']]
		if self.mType == modelpack.ModelType.RES:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Price', 'Lat', 'Long']]
			ret['Y'] = df[['PredPrice']]
		if self.mType == modelpack.ModelType.RENTS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Rent', 'Lat', 'Long']]
			ret['Y'] = df[['PredRent']]
		if self.mType == modelpack.ModelType.RMS:
			ret['X'] = df[['Timestamp', 'PredTimestamp', 'Price']]
			ret['Y'] = df[['Price']]

		return ret

	def initQuery(self):
		self.resultset = self.engine.execute(self.query)

	def getTickers(self):
		tickers = pd.read_sql("SELECT DISTINCT Ticker FROM Stocks;", con=engine)
		return tickers['Ticker'].tolist()