@ECHO OFF
qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" ./BudgetDataTest/Test.pro && mingw32-make.exe -C ./TestBuild && qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" ./RequestsTest/RequestsTest.pro && mingw32-make.exe -C ./TestBuild && "TestBuild/Test.exe" && "TestBuild/RequestsTest.exe"
