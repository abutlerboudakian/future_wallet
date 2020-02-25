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
	@effects:	Scrapes table with id GraphTable at the uri provided and returns a dataframe representing the table, parses data into DB format
	@returns:	pandas DataFrame object

'''
def parseTBondData(uri, driver):
	driver.get(uri)
	tableData = driver.find_element_by_id('GraphTable')
	table = tableData.get_attribute('outerHTML')
	data = pd.read_html(table)[0]
	data = data[data['MONTH/YEAR'] != 'Average' & data['MONTH/YEAR'] != 'Minimum' & data['MONTH/YEAR'] != 'Maximum']
	data = data.rename(columns={'MONTH/YEAR': 'Timestamp', '12 MAT': '12MAT', '1 MO. LIBOR': '1MoLIBOR', 'PRIME RATE': 'PrimeRate'})
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%m/%Y')
	return data

'''
	@params:	uri - The weblink to scrape
				root - The root directory to concatenate local hyperlinks with
				driver - Reference to chromedriver object
				depth(optional) - Current recursion depth
	@requires:	uri is an instantiated string representing a valid webpage containing either 2 tables of data or an unordered list of hyperlinks,
				root is an instantiated string representing the root directory for all possible local hyperlinks searched in function,
				driver is an instantiated chromedriver instance
	@modifies:	None
	@effects:	Scrapes 2 tables contained at the uri provided, or recursively searches an unordered list of hyperlinks if no tables are present up to a recursion depth of 2, parses for DB table format
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
		njdf = njdf[njdf['Deposit Products'] == 'Savings']
		njdf = njdf.drop(columns=['Deposit Products'])
		njdf = njdf.rename(columns={'National Rate': 'NationalRate', 'Rate Cap': 'RateCap'})
		jdf = pd.read_html(str(tables[1]))[0]
		jdf = jdf[jdf['Deposit Products'] == 'Savings']
		jdf = jdf.drop(columns=['Deposit Products'])
		jdf = jdf.rename(columns={'National Rate': 'NationalRate', 'Rate Cap': 'RateCap'})
		njdf['Timestamp'] = pd.to_datetime(((uri.split('/'))[-1]).split('.')[0], format='%Y-%m-%d')
		jdf['Timestamp'] = pd.to_datetime(((uri.split('/'))[-1]).split('.')[0], format='%Y-%m-%d')
	
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
	data = pd.read_html(table)[0]
	data['Timestamp'] = pd.to_datetime(data['Year'] + '-' + data['Period'], format='%Y-%m')
	data = data.drop(columns=['Year','Period'])
	return data

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
	min_year = 1997
	year_index = max_year
	years = []

	# Use 2017 data columns as starter
	data = parseCbpIncomeDataHelper("https://www2.census.gov/programs-surveys/cbp/datasets/2017/cbp17co.zip?#")

	# Get all years
	while year_index != min_year:
		years.append(year_index)
		year_index -= 1

	# Get and append data to cvs_data from all available years
	for year in years:
		strYear = str(year)
		df = parseCbpIncomeDataHelper("https://www2.census.gov/programs-surveys/cbp/datasets/" + strYear + "/cbp" + strYear[2:len(strYear)] + "co.zip?#")
		df['Timestamp'] = df.to_datetime(str(year), format='%Y')
		data = data.append(df)

	return data

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
	if 'EMP_NF' in df.columns:
		df = df[df['EMP_NF'] != 'D' & df['EMP_NF'] != 'S']
	if 'AP_NF' in df.columns:
		df = df[df['AP_NF'] != 'D' & df['AP_NF'] != 'S']
	df = df['FIPSTATE', 'FIPSCTY', 'NAICS', 'EMP', 'AP', 'EST']
	df = df.rename(columns={'FIPSTATE': 'StateCode', 'FIPSCTY': 'CountyCode', 'NAICS': 'IndustryCode', 'EMP': 'NumEmployees', 'AP': 'AnnualPayroll', 'EST': 'NumEstablishments'})

	# Close and delete 'data.txt' file
	f.close()
	os.remove('data.txt')

	return df

'''
	@params:	None
	@requires:	None 
	@modifies:	None
	@effects:	Returns DataFrame object of most recent (2017) NCAIS codes and their meaning
	@returns:	pandas DataFrame object
'''
def parseNAICSCode():
	'''
	NAICS code structure:
							NAICS Code Root
						  /	 |	  |	...	\
					    (11)(21) (22)	...
						/    |    |
					(111)	...  ...
					/
				(1111)	...
				/
			(11111)	...
			/	 |
	  (111110) (111120)
	  /           |
	(Soybean)   (Other flax seed) 	...

	Only one click is required to get from 2-digit NAICS code to 6-digit final code, so no recursion necessary.
	'''
	# Init dataframe
	naics_code = pd.DataFrame()
	print("Put column names in there ^")

	# Get HTML page of NAICS codes
	# Note" If this code is being used in 2022, this should be changed to
	# "https://www.census.gov/cgi-bin/sssd/naics/naicsrch?chart=2022" to reflect most recent data
	page = requests.get("https://www.census.gov/cgi-bin/sssd/naics/naicsrch?chart=2017")

	# Go through NAICS 2-digit code and get all 6-digit final codes from link.
	soup = bs4.BeautifulSoup(page.text, 'html.parser')
	for link in soup.find_all('a'):
		layer_1_code = link.get_text().split("-")

		if len(layer_1_code) > 2:
			continue
		for index in range(0, len(layer_1_code)):
			if layer_1_code[index].isdigit():
				if len(layer_1_code) == 2:
					print("Go inside and get the 6-digit codes and put inside df")
				else:
					print("Ditto")

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
  data = parseCsvData(uri)
  data = data[data['place_id'].str.isnumeric()]
  data = data[data['hpi_flavor'] == 'all-transactions']
  data['Timestamp'] = pd.to_datetime(data['yr'] + '-' + data['period'], format='%Y-%m')
  data = data.rename(columns={'place_id': 'ZIP', 'index_nsa': 'Index'})
  data = data['Timestamp', 'ZIP', 'Index']
  return data

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
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object, then puts it in format for DB table
	@returns:	pandas DataFrame object
'''	
def parseBondData(uri):
	data = parseCsvData(uri)
	data = data.rename(columns={'Date': 'Timestamp'})
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%d/%m/%Y')
	return data

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object
	@returns:	pandas DataFrame object
'''	
def parseRareMetalData(uri):
	data = parseCsvData(uri)
	data = data.rename(columns={'Date': 'Timestamp'})
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%Y-%m')
	return data

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