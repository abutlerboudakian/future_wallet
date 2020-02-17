from selenium import webdriver
import requests
import io
import pandas as pd

def parseTBondData(uri):
	options = webdriver.ChromeOptions()
	options.add_argument('headless')
	driver = webdriver.Chrome(chrome_options=options)
	driver.get(uri)
	tableData = driver.find_element_by_id('GraphTable')
	table = tableData.get_attribute('outerHTML')
	driver.quit()
	return pd.read_html(table)

def parseCsvData(uri):
	page = requests.get(uri)
	return pd.read_csv(io.StringIO(page.text))

def parseZillowRentData(uri):
	return parseCsvData(uri)

def parseHousingIndexData(uri):
	return parseCsvData(uri)

def parseCDData(uri):
	return parseCsvData(uri)

if __name__ == "__main__":
	# Alpha Vantage API Key: IJA5ZUY00CVDSFBK
	# TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates')
	# ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
	# HIdata = parseHousingIndexData('https://www.fhfa.gov/HPI_master.csv')
	CDdata = parseCDData('https://fred.stlouisfed.org/graph/fredgraph.csv?id=CD6NRJD')
	print(CDdata)
	