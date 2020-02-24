'''
	NOTE: This library requires an installation of Chromedriver present in the program directory to function as intended.

	Scrapers.py:
		Scraper Library with driver block for testing purposes.
		Acquires data from various FutureWallet determined data sources and formats them as
		pandas DataFrames for export to our DB.

	Authors:
		Andrew Butler-Boudakian
		Arron Look
		Farukh Saidmuratov
		Wendi Zheng
'''

# Import statements
from selenium import webdriver
import requests
import bs4
import io
import pandas as pd
import os

from zipfile import ZipFile
from os import listdir
import time
from random import randrange



'''
	@params:	uri - The weblink to scrape
				driver - Reference to chromedriver object
	@requires:	uri is an instantiated string representing a valid webpage containing a table with id GraphTable,
				driver is an instantiated chromedriver instance
	@modifies:	None
	@effects:	Scrapes table with id GraphTable at the uri provided and returns a dataframe representing the table
	@returns:	pandas DataFrame object

'''
def parseTBondData(uri, driver):
	driver.get(uri)
	tableData = driver.find_element_by_id('GraphTable')
	table = tableData.get_attribute('outerHTML')
	return pd.read_html(table)[0]

'''
	@params:	uri - The weblink to scrape
				root - The root directory to concatenate local hyperlinks with
				driver - Reference to chromedriver object
				depth(optional) - Current recursion depth
	@requires:	uri is an instantiated string representing a valid webpage containing either 2 tables of data or an unordered list of hyperlinks,
				root is an instantiated string representing the root directory for all possible local hyperlinks searched in function,
				driver is an instantiated chromedriver instance
	@modifies:	None
	@effects:	Scrapes 2 tables contained at the uri provided, or recursively searches an unordered list of hyperlinks if no tables are present up to a recursion depth of 2
	@returns:	tuple of 2 pandas DataFrame objects
'''
def parseSAData(uri, root, driver, depth=0):

	# Initialize empty Non-Jumbo and Jumbo Deposit DataFrames
	njdf = pd.DataFrame()
	jdf = pd.DataFrame()

	# Load Current webpage and get main content div
	driver.get(uri)
	div = driver.find_element_by_id('content')
	soup = BeautifulSoup(div.get_attribute('outerHTML'), 'lxml')

	# Attempt to locate tables in content div
	tables = soup.find_all('table')

	# If tables have been located:
	if len(tables) > 0:

		# Scrape tables and record date
		njdf = pd.read_html(str(tables[0]))[0]
		jdf = pd.read_html(str(tables[1]))[0]
		njdf['Date'] = pd.to_datetime(((uri.split('/'))[-1]).split('.')[0], format='%Y-%m-%d')
		jdf['Date'] = pd.to_datetime(((uri.split('/'))[-1]).split('.')[0], format='%Y-%m-%d')
	
	# If tables have not been located and current recursion depth is less than 2:
	elif depth <= 1:

		# Get all links in main reference list
		ul = soup.find_all('ul')[0]
		items = ul.find_all('li')

		# Loop through links
		for item in items:
			link = root + item.find('a')['href']

			# Access current link recursively and append result to local DataFrames
			(nj, j) = parseSAData(link, root, driver, depth+1)
			if njdf.empty:
				njdf = pd.DataFrame(nj)
			else:
				njdf = njdf.append(nj)
			if jdf.empty:
				jdf = pd.DataFrame(j)
			else:
				jdf = jdf.append(j)

	# Return DataFrames
	return (njdf, jdf)

'''
	@params:	uri - The weblink to scrape
				driver - Reference to chromedriver object
	@requires:	uri is an instantiated string representing a valid webpage containing a table with id seriesDataTable1,
				driver is an instantiated chromedriver instance
	@modifies:	None
	@effects:	Change option under select with name 'startYear' to value = '1990', perform an update on webpage by clicking 
				on input with id 'dv-submit'. Then, scrapes table with id seriesDataTable1 at the uri provided and returns a 
				dataframe representing the table
	@returns:	pandas DataFrame object
'''
def parseCpiData(uri, driver):
	driver.get(uri)
	driver.find_element_by_xpath("//select[@name='startYear']/option[@value='1990']").click()
	driver.find_element_by_xpath("//input[@id='dv-submit']").click()
	tableData = driver.find_element_by_id('seriesDataTable1')
	table = tableData.get_attribute('outerHTML')
	return pd.read_html(table)[0]

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object
	@returns:	pandas DataFrame object
'''
def parseCsvData(uri):
  page = requests.get(uri)
  return pd.read_csv(io.StringIO(page.text))

'''
	@params:	None
	@requires:	None
	@modifies:	None
	@effects:	Returns DataFrame object of CBP data from 2016 to 1986
	@returns:	pandas DataFrame object
'''

def parseCbpIncomeData():
	max_year = 2016
	min_year = 1986
	year_index = max_year
	years = []
	cvs_data = []

	# Use 2017 data columns as starter
	data2017 = parseCbpIncomeDataHelper("https://www2.census.gov/programs-surveys/cbp/datasets/2017/cbp17co.zip?#")

	# Get all years
	while year_index != min_year:
		years.append(year_index)
		year_index -= 1

	# Get and append data to cvs_data from all available years
	for year in years:
		strYear = str(year)
		cvs_data.append(parseCbpIncomeDataHelper("https://www2.census.gov/programs-surveys/cbp/datasets/" + strYear + "/cbp" +
												 strYear[2:len(strYear)] + "co.zip?#"))

	allData = pd.DataFrame()

	return allData

'''
	@params:	None
	@requires:	uri is in https://www2.census.gov/programs-surveys/cbp/datasets/
	@modifies:	None
	@effects:	Returns DataFrame object of target CBP Income data from 2016 to 1986
	@returns:	pandas DataFrame object
'''
def parseCbpIncomeDataHelper(uri):
	page = requests.get(uri)
	file = ZipFile(io.BytesIO(page.content))

	# Read zip file into text file
	f = open("data.txt", "w+")
	with file as myzip:
		myzip.writestr('/data.txt')
	df = pd.read_csv('data.txt')

	# Close and delete 'data.txt' file
	f.close()
	os.remove('data.txt')

	return df

'''
	@params:	None
	@requires:	uri is in "https://www.census.gov/cgi-bin/sssd/naics/"
	@modifies:	None
	@effects:	Returns DataFrame object of NCAIS codes and their meaning
	@returns:	pandas DataFrame object
'''
def parseNAICSCode(uri):
	return

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object, then puts it in format for DB table
	@returns:	pandas DataFrame object
'''
def parseZillowRentData(uri):
  data = parseCsvData(uri)
  data = pd.melt(data, id_vars=['RegionID', 'RegionName', 'City', 'State', 'Metro', 'CountyName', 'SizeRank'], var_name='Timestamp', value_name='Rent')
  data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%Y-%m')
  return data

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object
	@returns:	pandas DataFrame object
'''
def parseHousingIndexData(uri):
  return parseCsvData(uri)

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object, then puts it in format for DB table
	@returns:	pandas DataFrame object
'''
def parseCDData(uri):
	data = parseCsvData(uri)
	data = data.rename(columns={'DATE': 'Timestamp', 'CD6NRJD': 'Rate'})
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%d/%m/%Y')
	return data

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object
	@returns:	pandas DataFrame object
'''	
def parseBondData(uri):
	return parseCsvData(uri)

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object
	@returns:	pandas DataFrame object
'''	
def parseRareMetalData(uri):
	return parseCsvData(uri)

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object
	@returns:	pandas DataFrame object
'''	
def parseDividendData(uri):
	return parseCsvData(uri)

"""
  @param uri is a string denoting the default location of the dividend website
             namely https://dividata.com/ since https://dividata.com/stock/<ticker>/dividend
             gives us historic dividend data
  @param absPath is a string denoting the absolute path to the directory
                 containing a list of files of the format <ticker>.<market>.txt
  @param driver is the optional chromedriver
  @returns a dataframe containing Ticker | Date | Dividend Amount
           for each stock listed in the absPath directory
           a dataframe contaning Ticker | Market | Company name
"""
def parseStockDividend(uri, absPath, driver=None):
  close = None
  if driver is None:
    close = True
    driver = initChromeDriver()

  # Get the list of stocks from what's in the folder
  # or     /stock/<acronym>/dividend
  # I downloaded the Kaggle data here "/mnt/d/price-volume-data-for-all-us-stocks-etfs/Stocks"
  filenames = listdir(absPath)
  all_data = pd.DataFrame()
  tickerToName = pd.DataFrame(columns=["Ticker", "Market", "Company Name"])
  for i in range(len(filenames)): # For each stock
    filename = filenames[i]
    stock, market = filename.split(".")[:2]
    driver.get(uri + "/" + stock + "/dividend")
    tableData = driver.find_elements_by_tag_name("table")
    if len(tableData) != 0: # If stock has dividends
      table = tableData[-1].get_attribute('outerHTML')
      data = pd.read_html(table)[0] # pd dataframe acquired for this stock
      numEntries = data.shape[0]

      data = data.rename(columns={"Ex-Dividend Date":"Date"})
      data.insert(0, "Ticker", [stock]*numEntries, True)

      headers = driver.find_elements_by_tag_name("h2")
      headerData = headers[0].get_attribute("outerHTML")
      companyName = headerData.split(">")[1].split(" Dividend History")[0]

      tickerToName = tickerToName.append({"Ticker":stock, "Market":market, "Company Name":companyName}, ignore_index = True)
      print(i, stock)
      if i == 60:
        break
      all_data = pd.concat([all_data, data]) # add stock dividends to 
    time.sleep(randrange(1, 15)) # introduce some lag in between to reduce the number of requests
  
  if close is not None:
    driver.quit()
  return all_data, tickerToName

"""
  @param absPath is a string denoting the absolute path to /Stock or /ETF from the kaggle dataset
  @returns a panda dataframe of Ticker | Market | Date | Opening price | High | Low | Close | Volume | OpenInt
"""
def parseStockData(absPath):
  all_data = pd.DataFrame()
  for filename in listdir(absPath):
    stock, market = filename.split(".")[:2]
    with open(absPath + "\\" + filename) as file:
      try:
        data = pd.read_csv(file)
        numEntries = data.shape[0]
        data.insert(0, "Market", [market]*numEntries, True)
        data.insert(0, "Ticker", [stock]*numEntries, True)
        all_data = pd.concat([all_data, data])
      except pd.errors.EmptyDataError:
        pass
  return all_data

if __name__ == "__main__":
  # driver = initChromeDriver()
  # Alpha Vantage API Key: IJA5ZUY00CVDSFBK
  # TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates', driver)
  # ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
  # HIdata = parseHousingIndexData('https://www.fhfa.gov/HPI_master.csv')
  # CDdata = parseCDData('https://fred.stlouisfed.org/graph/fredgraph.csv?id=CD6NRJD')
  # BondData = parseBondData('https://datahub.io/core/bond-yields-us-10y/r/monthly.csv')
  # IncomeData = parseCBPIncomeData("https://www2.census.gov/programs-surveys/cbp/datasets/2017/cbp17cd.xlsx?#")
  # CpiData = parseCpiData('https://beta.bls.gov/dataViewer/view/timeseries/CUSR0000SA0')
  # SAdata = parseSAData('https://www.fdic.gov/regulations/resources/rates/previous.html', 'https://www.fdic.gov/regulations/resources/rates/', driver)
  # print(SAdata)
  # RaremetalData = parseRareMetalData('https://datahub.io/core/gold-prices/r/monthly.csv')
  # print(CpiData)
  # stockData = parseStockDividend("https://dividata.com/stock", "D:\\price-volume-data-for-all-us-stocks-etfs\\Stocks", driver) # There are around 7000 stocks to take note of
  # print(stockData, stockData.shape)
  # stockData = parseStockData("D:\\price-volume-data-for-all-us-stocks-etfs\\Stocks")
  # print(stockData, stockData.shape) # NOTE: Some of the txt files are empty
  DividendData = parseDividendData("https://datahub.io/core/s-and-p-500/r/data.csv")
  print(DividendData)
  # driver.quit()


# 5 min, 15 min | 8:34