import sqlalchemy as sa
import pandas as pd
from datetime import datetime, timedelta
import dbutil
from models import ModelType

class DatasetBuilder:
	def __init__(self):
		self.engine = dbutil.connect_engine('modeldata')

	def getModelData(mType, **kwargs):
		data = {}
		if mType == ModelType.WAGES:
			with engine.connect() as conn:
				if 'industryCode' in kwargs:
					data['industryCode'] = kwargs['industryCode']
					dataset = pd.read_sql(sql="""SELECT
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
												;""", con=conn)
					data['X'] = dataset[['Timestamp', 'PredTimestamp', 'Income', 'Lat', 'Long']]
					data['Y'] = dataset[['PredIncome']]
				else:
					return None

		elif mType == ModelType.INVEST:
			with engine.connect() as conn:
				if 'tickers' in kwargs:
					tickers = kwargs['tickers']
					savings = {}
					savingsSet = pd.read_sql("""SELECT
												s1.Timestamp AS Timestamp,
												s2.Timestamp AS PredTimestamp,
												s1.NationalRate AS Rate,
												s2.NationalRate AS PredRate
											FROM
												NJSAs s1,
												NJSAs s2
											WHERE
												s1.Timestamp < s2.Timestamp
											;""", con=conn)

					savingsSet = savingsSet.append(pd.read_sql("""SELECT
												s1.Timestamp AS Timestamp,
												s2.Timestamp AS PredTimestamp,
												s1.NationalRate AS Rate,
												s2.NationalRate AS PredRate
											FROM
												JSAs s1,
												JSAs s2
											WHERE
												s1.Timestamp < s2.Timestamp
											;""", con=conn))
					savings['X'] = savingsSet[['Timestamp', 'PredTimestamp', 'Rate']]
					savings['Y'] = savingsSet[['PredRate']]

					cds = {}
					cdSet = pd.read_sql("""SELECT
												c1.Timestamp AS Timestamp,
												c2.Timestamp AS PredTimestamp,
												c1.Rate AS Rate,
												c2.Rate AS PredRate
											FROM
												CDs c1,
												CDs c2
											WHERE
												c1.Timestamp < c2.Timestamp
											;""", con=conn)
					cds['X'] = cdSet[['Timestamp', 'PredTimestamp', 'Rate']]
					cds['Y'] = cdSet[['PredRate']]

					stocks = {}
					for t in ticker:
						stocks[t] = {}
						stockSet = pd.read_sql("""SELECT
												s1.Timestamp AS Timestamp,
												s2.Timestamp AS PredTimestamp,
												s1.Close AS Value,
												s2.Close AS PredValue
											FROM
												Stocks s1,
												Stocks s2
											WHERE
												s1.Ticker = '""" + t + """'
												s1.Ticker = s2.Ticker,
												s1.Timestamp < s2.Timestamp
											;""", con=conn)
						stocks[t]['X'] = stockSet[['Timestamp', 'PredTimestamp', 'Value']]
						stocks[t]['Y'] = stockSet[['PredValue']]

					bonds = {}
					bondSet = pd.read_sql("""SELECT
												b1.Timestamp AS Timestamp,
												b2.Timestamp AS PredTimestamp,
												b1.Rate AS Rate,
												b2.Rate AS PredRate
											FROM
												Bonds b1,
												Bonds b2
											WHERE
												b1.Timestamp > b2.Timestamp
											;""", con=conn)
					bonds['X'] = bondSet[['Timestamp', 'PredTimestamp', 'Rate']]
					bonds['Y'] = bondSet[['PredRate']]

					tbonds = {}
					tbSet = pd.read_sql("""SELECT
												b1.Timestamp AS Timestamp,
												b2.Timestamp AS PredTimestamp,
												b1.PrimeRate AS Rate,
												b2.PrimeRate AS PredRate
											FROM
												TBonds b1,
												TBonds b2
											WHERE
												b1.Timestamp > b2.Timestamp
											;""", con=conn)
					tbonds['X'] = tbSet[['Timestamp', 'PredTimestamp', 'Rate']]
					tbonds['Y'] = tbSet[['PredRate']]

					data = (savings, cds, stocks, bonds, tbonds)
				else:
					return None

		elif mType == ModelType.ASSETS:
			with engine.connect() as conn:
				res = {}
				resSet = pd.read_sql("""SELECT
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
										;""", con=conn)
				res['X'] = resSet[['Timestamp', 'PredTimestamp', 'Price', 'Lat', 'Long']]
				res['Y'] = resSet[['PredPrice']]

				rent = {}
				rentSet = pd.read_sql("""SELECT
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
										;""", con=conn)
				rent['X'] = rentSet[['Timestamp', 'PredTimestamp', 'Rent', 'Lat', 'Long']]
				rent['Y'] = rentSet[['PredRent']]

				rm = {}
				rmSet = pd.read_sql("""SELECT
											r1.Timestamp AS Timestamp,
											r2.Timestamp AS PredTimestamp,
											r1.Price AS Price,
											r2.Price AS PredPrice
										FROM
											RMI r1,
											RMI r2
										WHERE
											r1.Timestamp < r2.Timestamp
										;""", con=conn)
				rm['X'] = rmSet[['Timestamp', 'PredTimestamp', 'Price']]
				rm['Y'] = rmSet[['Price']]
				data = (res, rent, rm)
		else:
			raise 'Invalid model type specified.', mType

		return data