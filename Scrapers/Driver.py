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
import os
import pandas as pd
import Scrapers

'''
    @params:    headless(optional) - Determines whether chromedriver is a headless instance or not, enabled by default
    @requires:  chromedriver.exe is available in the current directory
    @modifies:  None
    @effects:   Creates a new chromedriver object (headless or not) and returns a reference to it.
    @returns:   New chromedriver object
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
    cstr = 'mssql+pyodbc://' + os.environ['FWUSER'] + ':' + os.environ['FWPASS'] + '@localhost:1433/modeldata?driver=ODBC+Driver+17+for+SQL+Server'
    engine = sa.create_engine(cstr, execution_options=dict(stream_results=True))

    driver = initChromeDriver()

    with engine.connect() as conn:
        sources = pd.read_sql(sql='SELECT * FROM Sources;', con=conn)

        for row in sources.itertuples():
            if(not row.Pushed):
                args = []
                if row.Link is not None:
                    links = row.Link.split('|')
                    args.extend(links)
                if row.Driver == 1:
                    args.append(driver)

                print('Getting ' + row.DestTable)
                if len(args) > 0:
                    data = getattr(Scrapers, row.Name)(*args)
                else:
                    data = getattr(Scrapers, row.Name)()
                table = row.DestTable

                if isinstance(data, tuple):
                    tables = table.split('|')
                    for i in range(0, len(tables)):
                        print(data[i].columns)
                        data[i].to_sql(tables[i], con=conn, if_exists='append', index=False)
                else:
                    data.to_sql(table, con=conn, if_exists='append', index=False)
                conn.execute('UPDATE Sources SET Pushed = 1 WHERE DestTable = \'' + row.DestTable + '\'')
                print('' + row.DestTable + ' pushed successfully!')


    driver.quit()
    engine.dispose()