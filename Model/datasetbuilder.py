import sqlalchemy as sa
import pandas as pd
import geopy
from datetime import datetime, timedelta
from models import ModelType

class DatasetBuilder:
	def __init__(self):
		cstr = 'mssql+pyodbc://' + str(sys.argv[1]) + ':' + str(sys.argv[2]) + '@localhost:1433/modeldata?driver=SQL+Server+Native+Client+11.0'
		self.engine = sa.create_engine(cstr)
		self.nomi = geopy.Nominatim(user_agent='FutureWallet')

	def getModelData(mType, args=None):
		if mType == ModelType.WAGES:
			with engine.connect() as conn:
				x = pd.read_sql(sql='SELECT * FROM Wages;', con=conn)

		elif mType == ModelType.INVEST:
			with engine.connect() as conn:

		elif mType == ModelType.ASSETS:
			with engine.connect() as conn:

		else:
			raise 'Invalid model type specified.', mType

		return data