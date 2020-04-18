QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += ../../Application/BudgetData.h \
           ../../Application/Requests.h \
           ../../Application/types.hpp
SOURCES +=  \
            ../../Application/BudgetData.cpp \
            ../../Application/Requests.cpp \
            tst_testrequests.cpp
