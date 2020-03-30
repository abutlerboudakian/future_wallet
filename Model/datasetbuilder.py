import sqlalchemy as sa
import pandas as pd
from datetime import datetime, timedelta
from models import ModelType

class DatasetBuilder:
	def __init__(self):
		cstr = 'mssql+pyodbc://' + str(sys.argv[1]) + ':' + str(sys.argv[2]) + '@localhost:1433/modeldata?driver=SQL+Server+Native+Client+11.0'
		self.engine = sa.create_engine(cstr)

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

		elif mType == ModelType.ASSETS:
			with engine.connect() as conn:

		else:
			raise 'Invalid model type specified.', mType

		return data