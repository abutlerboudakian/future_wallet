#include <QtTest>
#include <QCoreApplication>

#include "../../Application/BudgetData.h"
#include "../../Application/Requests.h"

// add necessary includes here

class TestRequests : public QObject
{
    Q_OBJECT

public:
    TestRequests();
    ~TestRequests();

private slots:
    // 11 tests
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
        Requests Req(nullptr, true);

        QJsonObject outcome, Wages, Invest, Assets;
        QJsonArray array;

        outcome = Req.getInputs(QString("Dummy"));
        Wages = outcome["wages"].toObject();
        Invest = outcome["invests"].toObject();
        Assets = outcome["assets"].toObject();

        QCOMPARE(outcome["userid"].toString(), QString("Dummy"));
        QCOMPARE(outcome["years"].toInt(), 11);

        QCOMPARE(Wages["hourly"].toBool(), true);
        QCOMPARE(Wages["hourspw"].toDouble(), 9.0);
        QCOMPARE(Wages["income"].toDouble(), 10.0);
        QCOMPARE(Wages["industryCode"].toString(), QString(" Abrasive Product Manufacturing"));
        QCOMPARE(Wages["loc"].toString(), QString("12180"));

        QCOMPARE(Invest["bonds"].toDouble(), 4.0);
        QCOMPARE(Invest["cd"].toDouble(), 5.0);
        QCOMPARE(Invest["savings"].toDouble(), 6.0);
        QCOMPARE(Invest["stocks"].toObject().count(), 1);
        QCOMPARE((Invest["stocks"].toObject())["a"].toDouble(), 7.0);
        QCOMPARE(Invest["tbonds"].toDouble(), 8.0);

        QCOMPARE(Assets["rm"].toDouble(), 3.0);
        array = Assets["rents"].toArray();
        QCOMPARE(array.count(), 1);
        QCOMPARE((array[0].toObject())["loc"].toString(), QString("12180"));
        QCOMPARE((array[0].toObject())["value"].toDouble(), 1.0);
        array = Assets["res"].toArray();
        QCOMPARE(array.count(), 1);
        QCOMPARE((array[0].toObject())["loc"].toString(), QString("12180"));
        QCOMPARE((array[0].toObject())["value"].toDouble(), 2.0);
    }
}

void TestRequests::TestAddBudget()
{
    // Tests addBudget
    {
        // bool addBudget(BudgetData * budget, QString userid);
        Requests Req(nullptr, true);
        BudgetData * budget = new BudgetData;
        QString userid = QString("Dummy");

        budget->addCategory(QString("Can Food"), 0.5);
        budget->addCategory(QString("Boxed Food"), 0.5);
        budget->setName(QString("Orange Budget"));

        QCOMPARE(Req.addBudget(budget, userid), true);

        delete budget;
    }
}

void TestRequests::TestLoadBudget()
{
    // Tests loadBudget
    {
        Requests Req(nullptr, true);
        QString budgetId = QString("Orange Things"), userid = QString("Dummy");
        BudgetData * outcome;

        outcome = Req.loadBudget(budgetId, userid);

        QCOMPARE(outcome->getName(), QString("Orange Things"));
        QCOMPARE(outcome->getCategoryValue(QString("Oranges")), 0.5);
        QCOMPARE(outcome->getCategoryValue(QString("Fanta")), 0.25);
        QCOMPARE(outcome->getCategoryValue(QString("Tangerines")), 0.25);

        QCOMPARE(outcome->removeCategory(QString("Oranges")), true);
        QCOMPARE(outcome->removeCategory(QString("Fanta")), true);
        QCOMPARE(outcome->removeCategory(QString("Tangerines")), true);

        QCOMPARE(outcome->isEmpty(), true);

        delete outcome;
    }
}

void TestRequests::TestListBudgets()
{
    // Tests listBudgets
    {
        Requests Req(nullptr, true);
        std::pair<bool, QStringList> outcome;
        QStringList budgets;

        outcome = Req.listBudgets(QString("Dummy"));

        QCOMPARE(outcome.first, true);
        budgets = outcome.second;
        QCOMPARE(budgets[0], QString("Orange Budget"));
        QCOMPARE(budgets[1], QString("Apple Budget"));
    }
}

void TestRequests::TestLogin()
{
    // Tests login
    {
        Requests Req(nullptr, true);
        QString username = QString("Dummy");
        QString password = QString("I am a password");

        QCOMPARE(Req.login(username, password), true);
    }
}

void TestRequests::TestLogout()
{
    // Tests logout
    {
        Requests Req(nullptr, true);
        QString username = QString("Dummy");

        Req.logout(username); // Can't really test this
    }
}

void TestRequests::TestRegister()
{
    // Tests Register
    {
        // bool Register(QString userId, QString Password);
        Requests Req(nullptr, true);
        QString username = QString("Dummy");
        QString password = QString("I am a password");

        QCOMPARE(Req.Register(username, password), true);
    }
}

void TestRequests::TestUpdateUserInfo()
{
    // Tests UpdateUserInfo

    // Original Info:
    QString username = QString("Dummy");
    QString password = QString("I am a new password");

    {   // Change Name
        // bool UpdateUserInfo(QString OriginalUserId, QString userId, QString Password);
        Requests Req(nullptr, true);

        QCOMPARE(Req.UpdateUserInfo(username, QString("Rabbit"), QString("")), true);
    }
    {   // Change Password
        Requests Req(nullptr, true);

        QCOMPARE(Req.UpdateUserInfo(username, QString(""), password), true);
    }
    {   // Change Both
        Requests Req(nullptr, true);

        QCOMPARE(Req.UpdateUserInfo(username, QString("Rabbit"), password), true);
    }
}

void TestRequests::TestGetIndustries()
{
    // Test getIndustries
    {
        Requests Req(nullptr, true);

        QCOMPARE(Req.getIndustries().isEmpty(), false);
    }
}

void TestRequests::TestGetStocks()
{
    // Test getStocks
    {
        Requests Req(nullptr, true);

        QCOMPARE(Req.getStocks().isEmpty(), false);
    }
}

QTEST_MAIN(TestRequests)

#include "tst_testrequests.moc"
