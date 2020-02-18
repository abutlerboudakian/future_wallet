from selenium import webdriver
import requests
import bs4
import io
import pandas as pd

from os import listdir

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


def parseTBondData(uri, driver):
	driver.get(uri)
	tableData = driver.find_element_by_id('GraphTable')
	table = tableData.get_attribute('outerHTML')
	return pd.read_html(table)

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
	return pd.read_html(table)

def parseCsvData(uri):
  page = requests.get(uri)
  return pd.read_csv(io.StringIO(page.text))

def parseCBPIncomeData(uri):
  # Need to reconcile CBP datasets, as format changes throughout the years, new counties  included, etc.
  return parseCsvDat(uri)

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

def parseStockData(uri, absPath, driver=None):
  """
  @param uri is a string denoting the default location of the dividend website
             namely https://dividata.com/ since https://dividata.com/stock/<ticker>
             gives us historic dividend data
  @param absPath is a string denoting the absolute path to the directory
                 containing a list of files of the format <ticker>.<market>.txt
  @param driver is the optional chromedriver
  @returns a dataframe containing Ticker | Market | Date | Dividend Amount
           for each stock listed in the absPath directory
  """
  close = None
  if driver is None:
    close = True
    driver = initChromeDriver()

  # Get the list of stocks from what's in the folder
  # or     /stock/<acronym>/dividend
  # I downloaded the Kaggle data here "/mnt/d/price-volume-data-for-all-us-stocks-etfs/Stocks"
  all_data = pd.DataFrame()
  count = 0
  for filename in listdir(absPath): # For each stock
    stock, market = filename.split(".")[:2]
    driver.get(uri + "/stock/" + stock)
    tableData = driver.find_elements_by_tag_name("table")
    if len(tableData) != 0: # If stock has dividends
      table = tableData[-1].get_attribute('outerHTML')
      data = pd.read_html(table)[0] # pd dataframe acquired for this stock
      numEntries = data.shape[0]

      data = data.rename(columns={"Ex-Dividend Date":"Date"})
      data.insert(0, "Market", [market]*numEntries, True)
      data.insert(0, "Ticker", [stock]*numEntries, True)
      count += 1
      if (count % 100) == 0:
        print(stock)

    all_data = pd.concat([all_data, data]) # add stock dividends to 
  
  if Close is not None:
    driver.quit()
  return all_data

if __name__ == "__main__":
  driver = initChromeDriver()
  # Alpha Vantage API Key: IJA5ZUY00CVDSFBK
  # TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates', driver)
  # ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
  # HIdata = parseHousingIndexData('https://www.fhfa.gov/HPI_master.csv')
  # CDdata = parseCDData('https://fred.stlouisfed.org/graph/fredgraph.csv?id=CD6NRJD')
  # BondData = parseBondData('https://datahub.io/core/bond-yields-us-10y/r/monthly.csv')
  # IncomeData = parseCBPIncomeData("https://www2.census.gov/programs-surveys/cbp/datasets/2017/cbp17cd.xlsx?#")
  # CpiData = parseCpiData('https://beta.bls.gov/dataViewer/view/timeseries/CUSR0000SA0', driver)
  # SAdata = parseSAData('https://www.fdic.gov/regulations/resources/rates/previous.html', 'https://www.fdic.gov/regulations/resources/rates/', driver)
  # print(SAdata)
  # RaremetalData = parseRaremetalData('https://datahub.io/core/gold-prices/r/monthly.csv')
  # print(CpiData)
  stockData = parseStockData("https://dividata.com/", "D:\\price-volume-data-for-all-us-stocks-etfs\\Stocks", driver)
  print(stockData)
  driver.quit()
