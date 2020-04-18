# future_wallet

Team Members:
  * Andrew Butler-Boudakian
  * Farukh Saidmuratov 
  * Arron Look
  * Wendi Zheng

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