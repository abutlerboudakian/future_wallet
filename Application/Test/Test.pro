QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += BudgetData.h \
           Requests.h \
           types.hpp
SOURCES +=  tst_testbudgetdata.cpp \
            BudgetData.cpp \
            Requests.cpp \
            tst_testrequests.cpp
