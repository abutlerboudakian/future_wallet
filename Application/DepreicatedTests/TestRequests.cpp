#include "TestRequests.h"

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

    /*QJsonObject getInputs(QString userid);

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
 
   QStringList getStocks();*/


