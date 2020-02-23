'''
	Driver.py:
		Driver module to control scrapers and manage database population.

	Authors:
		Andrew Butler-Boudakian
'''

import sqlalchemy as sa
import sys
import pandas as pd
import Scrapers


if __name__ == "__main__":
	cstr = 'mssql+pyodbc://' sys.argv[1] + ':' + sys.argv[2] + '@74.70.80.116:1433/modeldata'
	engine = sa.create_engine(cstr)
	conn = engine.connect()

	sources = pd.read_sql(sql='SELECT * FROM Sources;', con=conn)

	for row in sources.itertuples():
		continue


	conn.close()
	engine.dispose()