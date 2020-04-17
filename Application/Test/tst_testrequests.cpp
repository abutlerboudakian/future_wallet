#include <QtTest>
#include "Requests.h"

// add necessary includes here
// QCOMPARE(actual, expected)

class TestRequests : public QObject
{
    Q_OBJECT

public:
    TestRequests();
    ~TestRequests();

// Unit Tests are slot functions
private slots:
    // Inputs
    std::vector<double> * getPrediction(QString userId, QJsonObject Wages, QJsonObject Invest, QJsonObject Assets, int years);

    QJsonObject getInputs(QString userid);

    // Budgets
    bool addBudget(BudgetData * budget, QString userid);
    BudgetData * loadBudget(QString budgetId, QString userid);
    std::pair<bool, QStringList> listBudgets(QString userId);

    // Auth
    bool login(QString userId, QString Password);

    void logout(QString userId);

    bool Register(QString userId, QString Password);
    bool UpdateUserInfo(QString OriginalUserId, QString userId, QString Password);

    // Aux
    QStringList getIndustries();
    QStringList getStocks();
};

TestRequests::TestRequests()
{

}

TestRequests::~TestRequests()
{

}




QTEST_APPLESS_MAIN(TestRequests)

#include "tst_testrequests.moc"
