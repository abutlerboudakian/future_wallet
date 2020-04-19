#include <QtTest>
#include <QCoreApplication>
#include "../../Application/Requests.h"

// NOTE: This test requires app.py, which does some checking, to run.

class TestRequests : public QObject
{
    Q_OBJECT

public:
    TestRequests();
    ~TestRequests();

// Unit Tests are slot functions
private slots:
    // Inputs
    void TestGetPrediction();
    void TestGetInputs();
    void TestAddBudget();
    void TestLoadBudget();
    void TestListBudgets();
    void TestLogin();
    void TestLogout();
    void TestRegister();
    void TestUpdateUserInfo();
    void TestGetIndustries();
    void TestGetStocks();
};

TestRequests::TestRequests()
{

}

TestRequests::~TestRequests()
{

}

void TestRequests::TestGetPrediction()
{   // Test getPrediction endpoint
    {
        Requests Req(nullptr, true);
        int years = 1;
        std::string WagesData, InvestData, AssetsData;
        QJsonObject Wages, Invest, Assets;
        std::vector<double> * outcome;
        WagesData = "{\
                        \"hourly\": true,\
                        \"hourspw\": 9,\
                        \"income\": 10,\
                        \"industryCode\": \" Abrasive Product Manufacturing\",\
                        \"loc\": \"33333\"\
                     }";
        InvestData = "{\
                           \"bonds\": 4,\
                           \"cd\": 5,\
                           \"savings\": 6,\
                           \"stocks\": {\"a\":7},\
                           \"tbonds\": 8\
                      }";
        AssetsData = "{\
                           \"rents\": [{\
                               \"loc\": \"11111\",\
                               \"value\": 1\
                            }],\
                            \"res\": [{\
                                \"loc\": \"22222\",\
                                \"value\": 2\
                            }],\
                           \"rm\": 3\
                       }";
        Wages = QJsonDocument::fromJson(QString::fromStdString(WagesData).toUtf8()).object();
        Invest = QJsonDocument::fromJson(QString::fromStdString(InvestData).toUtf8()).object();
        Assets = QJsonDocument::fromJson(QString::fromStdString(AssetsData).toUtf8()).object();


        outcome = Req.getPrediction("Dummy", Wages, Invest, Assets, years);

        QCOMPARE((*outcome)[0], 100.0);
        QCOMPARE((*outcome)[1], 200.0);
        QCOMPARE((*outcome)[2], 300.0);
        QCOMPARE((*outcome)[3], 1);
    }
}

void TestRequests::TestGetInputs()
{
    // Tests getInputs
    {
        // QJsonObject getInputs(QString userid);
    }
}

void TestRequests::TestAddBudget()
{
    // Tests addBudget
    {
        // bool addBudget(BudgetData * budget, QString userid);
    }
}

void TestRequests::TestLoadBudget()
{
    // Tests loadBudget
    {
        // BudgetData * loadBudget(QString budgetId, QString userid);
    }
}

void TestRequests::TestListBudgets()
{
    // Tests listBudgets
    {
        //std::pair<bool, QStringList> listBudgets(QString userId);
    }
}

void TestRequests::TestLogin()
{
    // Tests login
    {
        // bool login(QString userId, QString Password);
    }
}

void TestRequests::TestLogout()
{
    // Tests logout
    {
        // void logout(QString userId);
    }
}

void TestRequests::TestRegister()
{
    // Tests Register
    {
        // bool Register(QString userId, QString Password);
    }
}

void TestRequests::TestUpdateUserInfo()
{
    // Tests UpdateUserInfo
    {
        // bool UpdateUserInfo(QString OriginalUserId, QString userId, QString Password);
    }
}

void TestRequests::TestGetIndustries()
{
    // Test getIndustries
    {
        // QStringList getIndustries();
    }
}

void TestRequests::TestGetStocks()
{
    // Test getStocks
    {
        // QStringList getStocks();*/
    }
}


QTEST_MAIN(TestRequests)

#include "tst_testrequests.moc"
