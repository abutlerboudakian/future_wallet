'''
	Driver.py:
		Driver module to control scrapers and manage database population.
		Takes DB connection username and password as system args 1 and 2.

	Authors:
		Andrew Butler-Boudakian
'''

# Import Statements
import sqlalchemy as sa
from selenium import webdriver
import sys
import pandas as pd
import Scrapers

'''
	@params:	headless(optional) - Determines whether chromedriver is a headless instance or not, enabled by default
	@requires:	chromedriver.exe is available in the current directory
	@modifies:	None
	@effects:	Creates a new chromedriver object (headless or not) and returns a reference to it.
	@returns:	New chromedriver object
'''
def initChromeDriver(headless=True):
  if headless:
    options = webdriver.ChromeOptions()
    options.add_argument('headless')
    options.add_argument("--log-level=3")
    driver = webdriver.Chrome(chrome_options=options)
    return driver
  else:
    driver = webdriver.Chrome(chrome_options=options)
    return driver

if __name__ == "__main__":
	cstr = 'mssql+pyodbc://' sys.argv[1] + ':' + sys.argv[2] + '@74.70.80.116:1433/modeldata'
	engine = sa.create_engine(cstr)
	conn = engine.connect()

	driver = initChromeDriver()

	sources = pd.read_sql(sql='SELECT * FROM Sources;', con=conn)

	for row in sources.itertuples():
		args = []
		links = row.Link.split('|')
		args.extend(links)
		if row.Driver == 1:
			args.append(driver)

		data = getattr(Scrapers, row.Name)(*args)
		table = row.DestTable

		if isinstance(data, tuple):
			tables = table.split('|')
			for d, t in data, tables:
				d.to_sql(t, con=conn, if_exists='append', index=False)
		else:
			data.to_sql(table, con=conn, if_exists='append', index=False)


	driver.quit()
	conn.close()
	engine.dispose()