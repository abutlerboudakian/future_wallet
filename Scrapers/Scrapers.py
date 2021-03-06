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
import random
from datetime import datetime, timedelta
import geopy
from geopy.exc import GeocoderTimedOut
import time

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
	data = data[(data['Month/Year'] != 'Average') & (data['Month/Year'] != 'Minimum') & (data['Month/Year'] != 'Maximum')]
	data = data.rename(columns={'Month/Year': 'Timestamp', '12 MAT': '12MAT', '1 Mo. Libor': '1MoLIBOR', 'Prime Rate': 'PrimeRate'})
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%m/%Y')
	data['12MAT'] = data['12MAT'].str.rstrip('%').astype('float') / 100.0
	data['1MoLIBOR'] = data['1MoLIBOR'].str.rstrip('%').astype('float') / 100.0
	data['PrimeRate'] = data['PrimeRate'].str.rstrip('%').astype('float') / 100.0
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
	soup = bs4.BeautifulSoup(div.get_attribute('outerHTML'), 'lxml')

	# Attempt to locate tables in content div
	tables = soup.find_all('table')

	# If tables have been located:
	if len(tables) > 0:

		# Scrape tables and record date
		njdf = pd.read_html(str(tables[0]))[0]
		njdf = njdf[njdf['Deposit Products'] == 'Savings']
		njdf = njdf.drop(columns=['Deposit Products'])
		njdf = njdf.rename(columns={'National Rate 1': 'NationalRate', 'Rate Cap 2': 'RateCap'})
		jdf = pd.read_html(str(tables[1]))[0]
		jdf = jdf[jdf['Deposit Products'] == 'Savings']
		jdf = jdf.drop(columns=['Deposit Products'])
		jdf = jdf.rename(columns={'National Rate 1': 'NationalRate', 'Rate Cap 2': 'RateCap'})
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
	print(data.columns)
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

# def tryGeocode(x, nomi):
# 	try:
# 		return nomi.geocode(x)
# 	except GeocoderTimedOut:
# 		time.sleep(1)
# 		return tryGeocode(x, nomi)

# def genLatLong(addrs):
# 	nomi = geopy.Nominatim(user_agent='FutureWallet')
# 	print(addrs)
# 	print(nomi.geocode(addrs[0]))
# 	lat = []
# 	lon = []
# 	for a in addrs:
# 		g = tryGeocode(a, nomi)
# 		if g is not None:
# 			lat.append(g.latitude)
# 			lon.append(g.longitude)
# 		else:
# 			lat.append(None)
# 			lon.append(None)
# 	return lat, lon

def parseCbpLocData():
	base = 'https://www2.census.gov/programs-surveys/cbp/technical-documentation/reference/state-county-geography-reference/georef'
	exts = {'17': [datetime(year=2017, month=1, day=1), datetime(year=2100, month=1, day=1)], '12':[datetime(year=2012, month=1, day=1), datetime(year=2016, month=1, day=1)], '02':[datetime(year=2002, month=1, day=1), datetime(year=2011, month=1, day=1)], '96_01':[datetime(year=1996, month=1, day=1), datetime(year=2001, month=1, day=1)]}
	zip2ll = pd.read_csv('US.csv', header=None, dtype={'ZIP':str}, names=['Country', 'ZIP', 'Location', 'State', 'StateAbbv', 'County', 'CountyNum', 'Lat', 'Long', 'Acc'])
	print(zip2ll)
	fips2zip = pd.read_csv('ZIP-COUNTY-FIPS_2017-06.csv', dtype={'STCOUNTYFP':str, 'ZIP':str})
	data_agg = pd.DataFrame()
	for e, ys in exts.items():
		page = requests.get(base + e + '.txt')
		data = pd.read_csv(io.StringIO(page.text), dtype={'fipstate':str, 'fipscty':str, 'st':str, 'cty':str})
		data['TimestampBegin'] = ys[0]
		data['TimestampEnd'] = ys[1]
		data = data.rename(columns={'fipstate': 'StateCode', 'fipscty': 'CountyCode', 'st':'StateCode', 'cty':'CountyCode', 'ctyname': 'Name'})
		data['STCOUNTYFP'] = data['StateCode'] + data['CountyCode']
		merged = pd.merge(pd.merge(data, fips2zip, on='STCOUNTYFP', sort=False), zip2ll, on='ZIP', sort=False)
		print(merged)
		data_agg = data_agg.append(merged[['TimestampBegin', 'TimestampEnd', 'StateCode', 'CountyCode', 'ZIP', 'Name', 'Lat', 'Long']])

	return data_agg

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
	data['Timestamp'] = pd.to_datetime('2017', format='%Y')

	# Get all years
	while year_index != min_year:
		years.append(year_index)
		year_index -= 1

	# Get and append data to cvs_data from all available years
	for year in years:
		strYear = str(year)
		df = parseCbpIncomeDataHelper("https://www2.census.gov/programs-surveys/cbp/datasets/" + strYear + "/cbp" + strYear[2:len(strYear)] + "co.zip?#")
		df['Timestamp'] = pd.to_datetime(strYear, format='%Y')
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
	f = ZipFile(io.BytesIO(page.content))
	
	df = pd.read_csv(f.open(ZipFile.namelist(f)[0]))
	df.columns = df.columns.str.lower()
	if 'emp_nf' in df.columns:
		df = df[(df['emp_nf'] != 'D') & (df['emp_nf'] != 'S')]
	if 'ap_nf' in df.columns:
		df = df[(df['ap_nf'] != 'D') & (df['ap_nf'] != 'S')]
	df = df[['fipstate', 'fipscty', 'naics', 'emp', 'ap', 'est']]
	df = df.rename(columns={'fipstate': 'StateCode', 'fipscty': 'CountyCode', 'naics': 'IndustryCode', 'emp': 'NumEmployees', 'ap': 'AnnualPayroll', 'est': 'NumEstablishments'})

	return df

def parseNAICSCodeCSV(fname):
	data = pd.read_csv(fname)
	data.columns = data.columns.str.lower()
	data = data.rename(columns={'current_naics_code':'IndustryCode', 'job_class':'Name', 'job_description':'Description'})
	return data

'''
	@params:	None
	@requires:	None 
	@modifies:	None
	@effects:	Returns DataFrame object of most recent (2017) NCAIS codes with columns indicating job name and job
	            description
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

	Ex: ---> = One click
	(11) ---> (111110) ---> (Job class, Prev NAICS code, Job Desc)
	'''
	NAICS_CODE_LEN_FULL = 6
	NAICS_CODE_LEN_SHORT = 2
	# Init dataframe with columns
	naics_code = pd.DataFrame(columns=["CURRENT_NAICS_CODE", "JOB_CLASS", "JOB_DESCRIPTION"])

	# Put previous NAICS codes on backlog for now. Previous NAICS codes determine multiple job titles, as opposed to one
	# naics_code["2012_NAICS_CODE"] = ""
	# naics_code["2007_NAICS_CODE"] = ""

	# Get HTML page of NAICS codes
	# Note" If this code is being used in 2022, this should be changed to "/cgi-bin/sssd/naics/naicsrch?chart=2022"
	# to reflect most recent data
	base = "https://www.census.gov"
	page = requests.get(base + "/cgi-bin/sssd/naics/naicsrch?chart=2017")

	# Go through NAICS 2-digit code and get all 6-digit final codes from link.
	soup = bs4.BeautifulSoup(page.text, 'html.parser')

	# For each link in the root page
	for link in soup.find_all('a'):
		# Split with '-' delimeter to get NAICS code in format 'XX-YY'
		layer_1_code = link.get_text().split("-")

		# For every element retrieved from the split array, check if its a 2-digit NAICS code.
		for index in range(0, len(layer_1_code)):
			if layer_1_code[index].isdigit():
				# Enter the 2-digit NAICS URI and get all 6-digit NAICS code under this node.
				inner_page = requests.get(base + link.get('href'))
				inner_soup = bs4.BeautifulSoup(inner_page.text, 'html.parser')

				for inner_link in inner_soup.find_all('a'):
					href_text = inner_link.get_text()
					# If href text is a 6-digit NAICS code, enter and get NAICS code info
					if href_text.isdigit():  # and len(href_text) == NAICS_CODE_LEN:
						# Wait a random amount of seconds from 1 <= n <= 4 to not get 503 error
						time.sleep(random.randint(1, 4))

						final_page = requests.get(base + inner_link.get('href'))
						final_soup = bs4.BeautifulSoup(final_page.text, 'html.parser')

						# Get inside div to get NAICS info
						middle_col = final_soup.find("div", {"id": "middle-column"})
						# Get job classification with h3 tag
						if len(href_text) == NAICS_CODE_LEN_SHORT or len(href_text) == NAICS_CODE_LEN_FULL:
							jobClassInd = 0
						else:
							jobClassInd = 1

						job_class_html = middle_col.findAll('h3')[jobClassInd]

						job_class = job_class_html.get_text()[len(href_text):len(job_class_html.get_text())]

						# Get job description before <br/>
						job_desc = job_class_html.next_sibling

						# Get 2007 NAICS and 2012 NAICS by getting table. Put this feature on the backlog for now.
						# naics_prev_html = middle_col.find("table")

						# Get naics code
						curr_naics = href_text
						print("curr_naics:", curr_naics)
						print("job_class", job_class)
						print("job_desc", job_desc)
						naics_code = naics_code.append(pd.Series([curr_naics, job_class, job_desc],
																 index=["CURRENT_NAICS_CODE", "JOB_CLASS",
																		"JOB_DESCRIPTION"]), ignore_index=True)
				break

	return naics_code

'''
	@params:	uri - The weblink to scrape
	@requires:	uri is an instantiated string representing a valid webpage containing a csv file
	@modifies:	None
	@effects:	Gets csv file located at uri and returns it as a pandas DataFrame object, then puts it in format for DB table
	@returns:	pandas DataFrame object
'''
def parseZillowRentData(uri):
  page = requests.get(uri)
  data = pd.read_csv(io.StringIO(page.text), dtype={'RegionName':str})
  data = pd.melt(data, id_vars=['RegionID', 'RegionName', 'City', 'State', 'Metro', 'CountyName', 'SizeRank'], var_name='Timestamp', value_name='Rent')
  data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%Y-%m')
  data = data.drop(columns=['SizeRank'])
  data = data.rename(columns={'RegionName':'ZIPCode','CountyName':'County'})
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
  data['Timestamp'] = pd.to_datetime(data['yr'].apply(str) + '-' + data['period'].apply(str), format='%Y-%m')
  data = data.rename(columns={'place_id': 'ZIP', 'index_nsa': 'Index'})
  data = data[['Timestamp', 'ZIP', 'Index']]
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
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%Y-%m-%d')
	data['Rate'] = pd.to_numeric(data['Rate'], errors='coerce') / 100
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
	data['Timestamp'] = pd.to_datetime(data['Timestamp'], format='%Y-%m-%d')
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
        data['Market'] = market
        data['Ticker'] = stock
        if all_data.empty:
        	all_data = data
        else:
        	all_data = all_data.append(data)
      except pd.errors.EmptyDataError:
        pass
  all_data = all_data[['Date', 'Ticker', 'Market', 'Open', 'Close', 'High', 'Low', 'Volume']]
  all_data = all_data.rename(columns={'Date':'Timestamp'})
  return all_data

# Example Driver Block:
#if __name__ == "__main__":
#   # driver = initChromeDriver()
#   # Alpha Vantage API Key: IJA5ZUY00CVDSFBK
#   # TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates', driver)
#   # ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
#   # HIdata = parseHousingIndexData('https://www.fhfa.gov/HPI_master.csv')
#   # CDdata = parseCDData('https://fred.stlouisfed.org/graph/fredgraph.csv?id=CD6NRJD')
#   # BondData = parseBondData('https://datahub.io/core/bond-yields-us-10y/r/monthly.csv')
	#IncomeData = parseCbpIncomeData()
#   # CpiData = parseCpiData('https://beta.bls.gov/dataViewer/view/timeseries/CUSR0000SA0')
#   # SAdata = parseSAData('https://www.fdic.gov/regulations/resources/rates/previous.html', 'https://www.fdic.gov/regulations/resources/rates/', driver)
#   # print(SAdata)
#   # RaremetalData = parseRareMetalData('https://datahub.io/core/gold-prices/r/monthly.csv')
#   # print(CpiData)
#   # stockData = parseStockDividend("https://dividata.com/stock", "D:\\price-volume-data-for-all-us-stocks-etfs\\Stocks", driver) # There are around 7000 stocks to take note of
#   # print(stockData, stockData.shape)
#   # stockData = parseStockData("D:\\price-volume-data-for-all-us-stocks-etfs\\Stocks")
#   # print(stockData, stockData.shape) # NOTE: Some of the txt files are empty

#   # NAICSdata = parseNAICSCode() # NOTE: this takes long because of time.sleep()
#   # NAICSdata.to_csv(path_or_buf='NAICSCode.csv', index=False)

#   DividendData = parseDividendData("https://datahub.io/core/s-and-p-500/r/data.csv")
#   print(DividendData)
#   # driver.quit()


# 5 min, 15 min | 8:34