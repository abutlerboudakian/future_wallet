from selenium import webdriver
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

if __name__ == "__main__":
	data = parseTBondData('https://www.firstrepublic.com/finmkts/historical-interest-rates')
	print(data)
	