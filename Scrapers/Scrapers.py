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
# from arron import *
import requests
from bs4 import BeautifulSoup
import io
import pandas as pd

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
		driver = webdriver.Chrome(chrome_options=options)
		return driver
	else:
		driver = webdriver.Chrome(chrome_options=options)
		return driver

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


def parseSAData(uri, root, driver, depth=0):
	njdf = pd.DataFrame()
	jdf = pd.DataFrame()
	driver.get(uri)
	div = driver.find_element_by_id('content')
	soup = BeautifulSoup(div.get_attribute('outerHTML'), 'lxml')
	tables = soup.find_all('table')
	if len(tables) > 0:
		njdf = pd.read_html(str(tables[0]))[0]
		jdf = pd.read_html(str(tables[1]))[0]
		njdf['Date'] = pd.to_datetime(((uri.split('/'))[-1]).split('.')[0], format='%Y-%m-%d')
		jdf['Date'] = pd.to_datetime(((uri.split('/'))[-1]).split('.')[0], format='%Y-%m-%d')
	elif depth <= 1:
		ul = soup.find_all('ul')[0]
		items = ul.find_all('li')
		for item in items:
			link = root + item.find('a')['href']
			(nj, j) = parseSAData(link, root, driver, depth+1)
			if njdf.empty:
				njdf = pd.DataFrame(nj)
			else:
				njdf = njdf.append(nj)
			if jdf.empty:
				jdf = pd.DataFrame(j)
			else:
				jdf = jdf.append(j)
	return (njdf, jdf)


def parseCpiData(uri, driver):
	driver.get(uri)
	driver.find_element_by_xpath("//select[@name='startYear']/option[@value='1990']").click()
	driver.find_element_by_xpath("//input[@id='dv-submit']").click()
	tableData = driver.find_element_by_id('seriesDataTable1')
	table = tableData.get_attribute('outerHTML')
	return pd.read_html(table)[0]

def parseCsvData(uri):
	page = requests.get(uri)
	return pd.read_csv(io.StringIO(page.text))

def parseCBPIncomeData(uri):
	# Need to reconcile CBP datasets, as format changes throughout the years, new counties included, etc.
	page = requests.get(uri)
	return pd.read_csv(io.StringIO(page.text), encoding = "ISO-8859-1")

def parseZillowRentData(uri):
	return parseCsvData(uri)

def parseHousingIndexData(uri):
	return parseCsvData(uri)

def parseCDData(uri):
	return parseCsvData(uri)
	
def parseBondData(uri):
	return parseCsvData(uri)
	
def parseRaremetalData(uri):
	return parseCsvData(uri)

if __name__ == "__main__":
	driver = initChromeDriver()
	# Alpha Vantage API Key: IJA5ZUY00CVDSFBK
	# TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates', driver)
	# ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
	# HIdata = parseHousingIndexData('https://www.fhfa.gov/HPI_master.csv')
	# CDdata = parseCDData('https://fred.stlouisfed.org/graph/fredgraph.csv?id=CD6NRJD')
	# BondData = parseBondData('https://datahub.io/core/bond-yields-us-10y/r/monthly.csv')
	# IncomeData = parseCBPIncomeData("https://www2.census.gov/programs-surveys/cbp/datasets/2017/cbp17cd.xlsx?#")
	# CpiData = parseCpiData('https://beta.bls.gov/dataViewer/view/timeseries/CUSR0000SA0')
	SAdata = parseSAData('https://www.fdic.gov/regulations/resources/rates/previous.html', 'https://www.fdic.gov/regulations/resources/rates/', driver)
	print(SAdata)
	# RaremetalData = parseRaremetalData('https://datahub.io/core/gold-prices/r/monthly.csv')
	# print(CpiData)
	# stockData = parseStockData("https://dividata.com/")
	driver.quit()
