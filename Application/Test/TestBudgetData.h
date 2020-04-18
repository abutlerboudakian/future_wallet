#ifndef TESTBUDGETDATA_H
#define TESTBUDGETDATA_H


#include <QtTest>
#include "../Application/BudgetData.h"

// add necessary includes here
// QCOMPARE(actual, expected)

class TestBudgetData : public QObject
{
    Q_OBJECT

public:
    TestBudgetData();
    ~TestBudgetData();

// Unit Tests are slot functions
private slots:
    void TestConstruction();
    void TestName();
    void TestDollars();
    void TestCategories();
    void TestIsEmpty();
    void TestRemaining();
    void TestDisplay();
    void TestChartMap();

};

#endif // TESTBUDGETDATA_H
