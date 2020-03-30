import sqlalchemy as sa
import os

def connect_engine(database):
	cstr = 'mssql+pyodbc://' + os.environ['FWUSER'] + ':' + os.environ['FWPASS'] + '@localhost:1433/' + database + '?driver=SQL+Server+Native+Client+11.0'
	return sa.create_engine(cstr)
