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

def parseZillowRentData(uri):
	page = requests.get(uri)
	return pd.read_csv(io.StringIO(page.text))

if __name__ == "__main__":
	#TBdata = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates')
	ZRdata = parseZillowRentData('http://files.zillowstatic.com/research/public/Zip/Zip_Zri_AllHomesPlusMultifamily.csv')
	print(ZRdata)
	