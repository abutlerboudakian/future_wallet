from selenium import webdriver
# from arron import *
import requests
# import bs4
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

def parseSAData(uri):
	options = webdriver.ChromeOptions()
	options.add_argument('headless')
	driver = webdriver.Chrome(chrome_options=options)
	driver.get(uri)


def parseCpiData(uri):
	options = webdriver.ChromeOptions()
	options.add_argument('headless')
	driver = webdriver.Chrome(chrome_options=options)
	driver.get(uri)
	driver.find_element_by_xpath("//select[@name='startYear']/option[@value='1990']").click()
	driver.find_element_by_xpath("//input[@id='dv-submit']").click()
	tableData = driver.find_element_by_id('seriesDataTable1')
	table = tableData.get_attribute('outerHTML')
	driver.quit()
	return pd.read_html(table)

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
	# Alpha Vantage API Key: IJA5ZUY00CVDSFBK
	# TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates')
	# ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
	# HIdata = parseHousingIndexData('https://www.fhfa.gov/HPI_master.csv')
	# CDdata = parseCDData('https://fred.stlouisfed.org/graph/fredgraph.csv?id=CD6NRJD')
	# BondData = parseBondData('https://datahub.io/core/bond-yields-us-10y/r/monthly.csv')
	# IncomeData = parseCBPIncomeData("https://www2.census.gov/programs-surveys/cbp/datasets/2017/cbp17cd.xlsx?#")
	CpiData = parseCpiData('https://beta.bls.gov/dataViewer/view/timeseries/CUSR0000SA0')
	SAdata = parseSavingsAcctData('https://www.fdic.gov/regulations/resources/rates/previous.html')
	# RaremetalData = parseRaremetalData('https://datahub.io/core/gold-prices/r/monthly.csv')
	# print(CpiData)
	stockData = parseStockData("https://dividata.com/")
	
