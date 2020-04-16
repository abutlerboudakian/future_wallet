#include <QtTest/QtTest>
#include "BudgetData.h"

class TestBudgetData: public QObject
{
    Q_OBJECT

// Unit Tests are slot functions
private slots:
    void construction();
    void Name();
    void Dollars();
    void Categories();
    void isEmpty();
    /*void Remaining();
    void Display();
    void ChartMapTest();*/
};

void TestBudgetData::construction()
{   // Test construction
    {
        BudgetData budget;
        QVERIFY2(budget.isEmpty(), "Budget on the stack should be empty.");
    }
    {
        BudgetData * budget;
        budget = new BudgetData;
        QVERIFY2((budget != nullptr) && (budget->isEmpty()), "Budget should be created.");
        delete budget;
    }
}

void TestBudgetData::Name()
{   // Test setName and getName
    BudgetData budget;
    QCOMPARE(budget.getName(), QString("")); // Should be empty string
    budget.setName(QString("Test"));
    QCOMPARE(budget.getName(), QString("Test"));
}

void TestBudgetData::Dollars()
{   // Test setDollar and getDollar
    BudgetData budget;
    QCOMPARE(budget.getDollar(), 0);

    budget.setDollar(150.05);
    QCOMPARE(budget.getDollar(), 150.05);
}

void TestBudgetData::Categories()
{   // Test addCategory, removeCategory, getCategoryValue
    {
        BudgetData budget;
        QCOMPARE(budget.getCategoryValue(QString("Cat1")), -1); // Make sure it doesn't exist first

        QCOMPARE(budget.removeCategory(QString("Cat1")), false); // Make sure it's not removed either

        QCOMPARE(budget.addCategory(QString("Cat1"), 0.5), true); // Add it
        QCOMPARE(budget.getCategoryValue(QString("Cat1")), 0.5); // Make sure it got aded
    }

    {   // add Category
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0), true);
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0), false);
        QCOMPARE(budget.addCategory(QString("Cat 1"), 1), false);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 1), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.01), false);

        // get Category
        QCOMPARE(budget.getCategoryValue(QString("Cat 1")), 0);
        QCOMPARE(budget.getCategoryValue(QString("Cat 2")), 1);
        QCOMPARE(budget.getCategoryValue(QString("Cat 3")), -1);
    }

    {   // add Category
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.01), true);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), true);

        QCOMPARE(budget.getCategoryValue(QString("Cat 1")), 0.05);
        QCOMPARE(budget.getCategoryValue(QString("Cat 2")), 0.75);
        QCOMPARE(budget.getCategoryValue(QString("Cat 3")), 0.01);
        QCOMPARE(budget.getCategoryValue(QString("Cat 4")), 0.19);
    }

    {   // add Category
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.01), true);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), true);
        QCOMPARE(budget.addCategory(QString("Cat 5"), 0.001), false);

        QCOMPARE(budget.getCategoryValue(QString("Cat 1")), 0.05);
        QCOMPARE(budget.getCategoryValue(QString("Cat 2")), 0.75);
        QCOMPARE(budget.getCategoryValue(QString("Cat 3")), 0.01);
        QCOMPARE(budget.getCategoryValue(QString("Cat 4")), 0.19);
    }

    {   // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.01), false);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), false);

        QCOMPARE(budget.getCategoryValue(QString("Cat 1")), 0.05);
        QCOMPARE(budget.getCategoryValue(QString("Cat 2")), 0.75);
        QCOMPARE(budget.getCategoryValue(QString("Cat 3")), 0.01);
        QCOMPARE(budget.getCategoryValue(QString("Cat 4")), 0.19);

        // remove Category
        QCOMPARE(budget.removeCategory(QString("Cat 3")), true);
        QCOMPARE(budget.removeCategory(QString("Cat 3")), false);
        QCOMPARE(budget.getCategoryValue(QString("Cat 1")), 0.05);
        QCOMPARE(budget.getCategoryValue(QString("Cat 2")), 0.75);
        QCOMPARE(budget.getCategoryValue(QString("Cat 3")), -1);
        QCOMPARE(budget.getCategoryValue(QString("Cat 4")), 0.19);
    }
}

void TestBudgetData::isEmpty()
{
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.isEmpty(), true);
    }
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);

        QCOMPARE(budget.isEmpty(), false);
    }
}

/*void TestBudgetData::Remaining()
{   // Test getRemaining
    double getRemaining() const;

}

void TestBudgetData::Display()
{   // Test getBudgetString
    QString getBudgetString() const;
}

void TestBudgetData::ChartMapTest()
{   // Test getBudgetChartMap
    const ChartMap * getBudgetChartMap() const;
}*/

