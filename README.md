# future_wallet

Team Members:
  * Andrew Butler-Boudakian
  * Farukh Saidmuratov 
  * Arron Look
  * Wendi Zheng

## Deploying the Server
FutureWallet's REST Server runs on Ubuntu using virtualenv, Flask, uWSGI, and Nginx.
To set up the server, you will need to install python3, all dependencies, and the modules/services listed above.
Then, you will need to create a service on the system for the application using systemctl.
Once that is complete, you need to create another configuration file in nginx's sites-available folder.

## Test Build Instructions
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
We had to separate the tests because I couldn't figure out how Qt included the QApplication in the main function that they abstractly generate. The Requests class requires the QApplication class for the QEventLoop objects.

Or you can just run [MakeAndRunTest.bat](./Application/Test/MakeAndRunTest.bat) in teh [Test](./Application/Test) directory assuming you have the environment variables set up, and the same make version as the developers.

[//]: # (For Windows User dev team, make is C:\Qt\Tools\mingw730_64\bin\mingw32-make.exe)

## Create Future Wallet App Instructions

If FutureWalletInstaller.exe/FutureWalletInstaller.app does not already exist in your repo navigate to /Application/fwInstaller

If on windows, run:

```
 ..\..\bin\binarycreator.exe -c config\config.xml -p packages FutureWalletInstaller.exe
 ```

Otherwise if on Mac/Linux: 

```
../../bin/binarycreator -c config/config.xml -p packages FutureWalletInstaller
```

This will create a .exe file or a .app file respective to Windows or Mac/Linux OS. Run installer to get an instance of the Future Wallet app on your machine. Should be connected to RPI VPN for the current version of Future Wallet.
