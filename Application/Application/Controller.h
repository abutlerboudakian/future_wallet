#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStackedWidget>
#include "ChartTemplate.h"
#include "BudgetData.h"

#include "DashBoard.h"
#include "LaunchMenu.h"
class BudgetPage;
#include "BudgetPage.h"
#include "predictionInputAssets.h"
#include "predictionInputWages.h"
#include "predictionInputInvest.h"
#include "InputBudget.h"
class AccountManagement;
#include "AccountManagement.h"
#include "Registration.h"

#include "Requests.h"
#include <QMessageBox>

class MainApplication;
#include "mainapplication.h"
class Menu;
#include "Menu.h"

enum Views
{
    Login = 0,
    Dashboard = 1,
    WagePredict = 2,
    InvestPredict = 3,
    AssetPredict = 4,
    BudgetInput = 5,
    RegistrationPage = 6
};

class Controller
{
  public:
    Controller();
    ~Controller();

    void setViews(QStackedWidget * Views);

    // View Switching
    void switchToDashBoard();
    void switchToLogin();
    void switchToBudgetPage();
    void switchToInputWages();
    void switchToInputInvest();
    void switchToInputAsset();
    void switchToInputBudget();
    void switchToRegisterPage();

    void switchToAccountManage();

    void closeBudgetPage();
    void closeAccountManage();

    // Charts
    QChartView * getPieChart(QString ChartName, const ChartMap * data);
    QChartView * getBarGraph(QString ChartName, const ChartMap * data);
    QChartView * getLineGraph(QString ChartName, const LineMap * data);

    const std::vector<double> * getMetricsData();
    const BudgetData * getBudgetData(QString budgetId);
    void setSelectedBudget(QString budgetId);

    // Endpoints
    void getPrediction();
    bool getInputs();

    void addBudget(BudgetData * budget);
    QStringList getBudgetList();

    void login(QString userid, QString Password);
    void logout();
    void Register(QString userid, QString Password);
    void UpdateUserInfo(QString newUserId = QString(""), QString Password = QString(""));

    QStringList getIndustries();
    QStringList getTickers();

    void setMain(MainApplication * main);

  private:
    QStackedWidget * Views;
    MainApplication * main;
    Menu * menubar;

    // Template Method Creators for the charts
    PieGUI * PieCreator;
    BarGUI * BarCreator;
    LineGUI * LineCreator;

    BudgetPage * BudgetView;
    AccountManagement * manage;

    // ModelData
    BudgetData * budget;
    // metrics;
    std::vector<double> * metrics; // [wages growth, investment growth, asset growth, years]
    int years; // Years for recent prediction

    // BudgetModal Boolean
    bool BudgetModal = false;

    // AccountModal Boolean
    bool AccountModal = false;

    Requests * ReqObj;

    QString userid;
};

#endif // CONTROLLER_H
