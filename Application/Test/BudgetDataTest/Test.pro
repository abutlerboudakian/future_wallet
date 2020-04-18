QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += ../../Application/BudgetData.h \
           TestBudgetData.h \
           ../../Application/types.hpp
SOURCES +=  \
            ../../Application/BudgetData.cpp \
            TestBudgetData.cpp \
            tst_RunTests.cpp
