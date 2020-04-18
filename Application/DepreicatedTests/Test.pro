QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += ../Application/BudgetData.h \
           ../Application/Requests.h \
           TestBudgetData.h \
           TestRequests.h \
           ../Application/types.hpp
SOURCES +=  \
            ../Application/BudgetData.cpp \
            ../Application/Requests.cpp \
            TestBudgetData.cpp \
            TestRequests.cpp \
            tst_RunTests.cpp
