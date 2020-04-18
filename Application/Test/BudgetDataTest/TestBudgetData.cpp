#include "TestBudgetData.h"

TestBudgetData::TestBudgetData()
{

}

TestBudgetData::~TestBudgetData()
{

}


void TestBudgetData::TestConstruction()
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

void TestBudgetData::TestName()
{   // Test setName and getName
    BudgetData budget;
    QCOMPARE(budget.getName(), QString("")); // Should be empty string
    budget.setName(QString("Test"));
    QCOMPARE(budget.getName(), QString("Test"));
}

void TestBudgetData::TestDollars()
{   // Test setDollar and getDollar
    BudgetData budget;
    QCOMPARE(budget.getDollar(), 0);

    budget.setDollar(150.05);
    QCOMPARE(budget.getDollar(), 150.05);
}

void TestBudgetData::TestCategories()
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
        QCOMPARE(budget.getCategoryValue(QString("Cat 5")), -1);
    }

    {   // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.01), true);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), true);

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

void TestBudgetData::TestIsEmpty()
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

void TestBudgetData::TestRemaining()
{   // Test getRemaining
    {
        BudgetData budget;
        QCOMPARE(budget.getRemaining(), 100.0);
    }
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.4), true);

        QCOMPARE(budget.getRemaining(), 60.0);
    }
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.99), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.001), true);

        QCOMPARE(budget.getRemaining(), 0.9);
    }
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.99), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.01), true);

        QCOMPARE(budget.getRemaining(), 0.0);
    }
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.009), true);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), true);
        QCOMPARE(budget.addCategory(QString("Cat 5"), 0.001), true);

        QCOMPARE(budget.getRemaining(), 0.0);
    }
}

void TestBudgetData::TestDisplay()
{   // Test getBudgetString -- Bad since it could be nondeterministic order
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.009), true);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), true);
        QCOMPARE(budget.addCategory(QString("Cat 5"), 0.001), true);

        QString Expected = QString("Cat 4\t\t19% |\t $0\n") +
                           QString("Cat 3\t\t0.9% |\t $0\n") +
                           QString("Cat 5\t\t0.1% |\t $0\n") +
                           QString("Cat 1\t\t5% |\t $0\n") +
                           QString("Cat 2\t\t75% |\t $0\n") +
                           QString("\t Total Amount of Income: $ 0\n") +
                           QString("\t Total Allocated Income: $ 0\n") +
                           QString("\t Total Remaining Income: $ 0\n");
        QCOMPARE(budget.getBudgetString(), Expected);
    }
}

void TestBudgetData::TestChartMap()
{   // Test getBudgetChartMap -- Bad since it could be nondeterministic order
    {
        // setup
        BudgetData budget;
        QCOMPARE(budget.addCategory(QString("Cat 1"), 0.05), true);
        QCOMPARE(budget.addCategory(QString("Cat 2"), 0.75), true);
        QCOMPARE(budget.addCategory(QString("Cat 3"), 0.01), true);
        QCOMPARE(budget.addCategory(QString("Cat 4"), 0.19), true);

        ChartMap * Perfect = new ChartMap;
        Perfect->insert(std::pair<std::string, double>("Cat 1", 0.05));
        Perfect->insert(std::pair<std::string, double>("Cat 2", 0.75));
        Perfect->insert(std::pair<std::string, double>("Cat 3", 0.01));
        Perfect->insert(std::pair<std::string, double>("Cat 4", 0.19));

        // getBudgetChartMap
        const ChartMap * Sample;
        Sample = budget.getBudgetChartMap();
        ChartMap::const_iterator it = Sample->begin();
        QCOMPARE(it != Sample->end(), true);
        QCOMPARE(it->first, "Cat 4");
        QCOMPARE(it->second, 0.19);
        it++;
        QCOMPARE(it->first, "Cat 3");
        QCOMPARE(it->second, 0.01);
        it++;
        QCOMPARE(it->first, "Cat 1");
        QCOMPARE(it->second, 0.05);
        it++;
        QCOMPARE(it->first, "Cat 2");
        QCOMPARE(it->second, 0.75);
        it++;
        QCOMPARE(it, Sample->end());

    }
}
/*QTEST_APPLESS_MAIN(TestBudgetData)

#include "tst_testbudgetdata.moc"*/
