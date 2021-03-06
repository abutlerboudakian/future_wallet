# future_wallet

Team Members:
  * Andrew Butler-Boudakian
  * Farukh Saidmuratov 
  * Arron Look
  * Wendi Zheng
  
## Description
FutureWallet is a desktop application used for income prediction and budgeting that income prediction.

**NOTICE** : Currently, in order to use the client application after installation, the server must still be running (on RPI Campus) under the host name butlea2.cs.rpi.edu and the user must be located on RPI campus or connected to the RPI campus via a vpn. If you deploy the server, train the models, and store the data on a separate server, change the **location** variable in Requests.h to that host name, recompile the installer, and reinstall your version of the client.

## Installer
### Using the Installer for Windows to use the application.
#### Installing Future Wallet App
**Warning:** The installer unloads all the object files and other application files in the directory you specify. On uninstallation, Qt removes all contents from that directory, including any existing files from before or after installation in that directory. Make sure to install specifying a new folder to prevent any loss of data. **The installer warns you about this as well.**
1. On a Windows OS, go to the [Installer Folder](./Windows%20Installer)
2. Launch the executable installer, and follow the instructions to install the Future Wallet application. (Remember to select all components)
3. Once the installer is finished, an executable **Application.exe** can be found in the directory you specified to install to. That is the Future Wallet App.

#### Uninstalling Future Wallet App
1. In the directory you installed the Future Wallet App in, launch the **management tool.exe**.
2. Select the **remove all components** option and let the uninstaller run.

## Making Installer Executable
### Devlopment Instructions for Compiling the Future Wallet App Installer executable
1. Create a package directory for the installable components. For more information, see Package Directory (https://doc.qt.io/qtinstallerframework/ifw-component-description.html).
2. Create a configuration file called config.xml in the config directory. It contains information about how to build the installer binaries and online repositories. For more information about the file format and available settings, see Configuration File (https://doc.qt.io/qtinstallerframework/ifw-globalconfig.html).
3. Create a package information file called package.xml in the config\meta directory. It contains settings for deployment and the installation process. For more information, see Meta Directory (https://doc.qt.io/qtinstallerframework/ifw-component-description.html#meta-directory).
4. Create installer content and copy it to the package directory. For more information, see Data Directory (https://doc.qt.io/qtinstallerframework/ifw-component-description.html#data-directory).
5. To add dependencies, go to Qt\5.14.1\mingw73_32\bin, and run the following command line:
```
   Qt\5.14.1\mingw73_32\bin>windeployqt.exe <location of application content>
```
   After that, add all .dll files from Qt\5.14.1\mingw73_32\bin to the location of application content.

6. At this point you should have the following directory structure:
```
   FutureWalletInstaller  --> config -> config.xml & package.xml
                         |          
                          --> packages -> FutureWallet  --> data --> content of application
                                                       |
                                                        --> meta --> license.txt & package.xml
```                                                        
                                                        
7. Go to Qt\Tools\QInstallerFramework\3.2\bin and use the binarycreator tool to create the installer. 
```
 <location-of-ifw>\binarycreator.exe -t <location-of-ifw>\installerbase.exe -p <package_directory> -c <config_directory>\<config_file>  <installer_name>
 ```

## Deploying the Server
FutureWallet's REST Server runs on Ubuntu using virtualenv, Flask, uWSGI, and Nginx.
To set up the server, you will need to install python3, all dependencies, and the modules/services listed above.
Then, you will need to create a service on the system for the application using systemctl.
Once that is complete, you need to create another configuration file in nginx's sites-available folder.

FutureWallet's SQL Server runs on Microsoft SQL Server 2019, which can be installed on the same machine as a virtual machine
or can be installed on a separate Windows installation. The Server runs as a named instance, called FUTUREWALLETDB.

There are 2 separate databases on the SQL server instance: `accountdata` and `modeldata`

The schema for `accountdata` is as follows:
```
Users(userid varchar(MAX), password varchar(MAX))
BudgetCategories(userid varchar(MAX), budgetid varchar(MAX), catname varchar(MAX), weight float)
Inputs(userid varchar(MAX), industryCode varchar(MAX), loc varchar(MAX), income float, hourly bit, hourspw float, savings float, cd float, bonds float, tbonds float, rm float, years int)
UserStocks(userid varchar(MAX), ticker varchar(MAX), shares float)
UserRes(userid varchar(MAX), loc varchar(MAX), price float)
UserRent(userid varchar(MAX), loc varchar(MAX), rent float)
```

The schema for `modeldata` is as follows:
```
Bonds(Timestamp datetime, Rate float)
CBPIncome(Timestamp datetime, StateCode varchar(MAX), CountyCode varchar(MAX), IndustryCode varchar(MAX), NumEmployees int, AnnualPayroll money, NumEstablishments int)
CDs(Timestamp datetime, Rate float)
CPI(Timestamp datetime, Label varchar(MAX), Value float)
FIPS(TimestampBegin datetime, TimestampEnd datetime, StateCode varchar(MAX), CountyCode varchar(MAX), ZIP varchar(MAX), Name varchar(MAX), Lat float, Long float)
HI(Timestamp datetime, ZIP varchar(MAX), [Index] float)
JSAs(Timestamp datetime, NationalRate float, RateCap float)
NAICS(IndustryCode varchar(MAX), Name varchar(MAX), Description varchar(MAX))
NJSAs(Timestamp datetime, NationalRate float, RateCap float)
RMI(Timestamp datetime, Price float)
Sources(Name varchar(MAX), Link varchar(MAX), Driver bit, DestTable varchar(MAX), Pushed bit)
Stocks(Timestamp datetime, Ticker varchar(MAX), Market varchar(MAX), [Open] money, [close] money, High money, Low money, Volume money)
TBonds(Timestamp datetime, [12MAT] float, [1MoLIBOR] float, PrimeRate float)
ZillowRent(Timestamp datetime, RegionID int, ZIPCode varchar(MAX), City varchar(MAX), State varchar(MAX), Metro varchar(MAX), County varchar(MAX), Rent float)
```

To collect the data that has been used for training, run Driver.py in the Scrapers folder.
Some additional files and directories must be present during runtime for all data to be acquired properly:

`parseNAICSCodeCSV` requires an absolute path to `NAICSCode.csv` in `Link` under `Sources` table in `modeldata` database
`parseStockData` requires an absolute path to `Stocks`, a directory containing all stock data from `https://www.kaggle.com/borismarjanovic/price-volume-data-for-all-us-stocks-etfs` in `Link` under `Sources` table in `modeldata` database


Training of the neural networks for the model can be done through modeldriver.py in the Models folder.

Database connection for runtime and model training is managed through `dbutil.py`.
To configure credentials for DB access, set your environment variables to the following:
```
FWUSER=username
FWPASS=password
```

## Test Build Instructions
Note that Qt and some executable make are required to build the unit tests.
In order to run the unit tests for the BudgetData and Requests classes, do the following:
In the [Test](./Application/Test) directory, run the following commands:
```
qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" ./BudgetDataTest/Test.pro
make -C ./TestBuild
qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" ./RequestsTest/RequestsTest.pro
make -C ./TestBuild
python -m flask run &
./TestBuild/Test.exe
./TestBuild/RequestsTest.exe
```
Make sure to run the flask app (mock endpoints) in a separate shell or in the background.
We had to separate the tests because we couldn't figure out how Qt included the QApplication in the main function that they abstractly generate. The Requests class requires the QApplication class for the QEventLoop objects.

Or you can just run [MakeAndRunTest.bat](./Application/Test/MakeAndRunTest.bat) in the [Test](./Application/Test) directory assuming you have the environment variables set up, and the same make version as the developers.

[//]: # (For Windows User dev team, make is C:\Qt\Tools\mingw730_64\bin\mingw32-make.exe)

## Coding Standards
- Variables will be **camelCase**
- **Spaces** will be used instead of **tabs** for indentation
- All methods will have POS (Principles of Software) specification or have a short description (if a POS specification does not make sense).
-- e.g. @param, @requires, @modifies, @effects, @throws, etc.
