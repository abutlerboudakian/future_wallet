import sqlalchemy as sa
import os

def connect_engine(database):
	cstr = 'mssql+pyodbc://' + os.environ['FWUSER'] + ':' + os.environ['FWPASS'] + '@74.70.80.116:1433/' + database + '?driver=SQL+Server+Native+Client+11.0'
	return sa.create_engine(cstr, execution_options=dict(stream_results=True))
