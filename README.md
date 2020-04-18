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
qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" Test.pro
make -C ./TestBuild
./TestBuild/Test.exe
```
[//]: # (For Windows User dev team, make is C:\Qt\Tools\mingw730_64\bin\mingw32-make.exe)