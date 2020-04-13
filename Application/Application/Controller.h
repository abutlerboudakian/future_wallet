#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStackedWidget>
#include "ChartTemplate.h"
#include "BudgetData.h"

#include "DashBoard.h"
#include "LaunchMenu.h"
#include "BudgetPage.h"
#include "predictionInputAssets.h"
#include "predictionInputWages.h"
#include "predictionInputInvest.h"

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
    AssetPredict = 4
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

    void closeBudgetPage();

    // Charts
    QChartView * getPieChart(const ChartMap * data);
    QChartView * getBarGraph(const ChartMap * data);
    QChartView * getLineGraph(const LineMap * data);

    const std::vector<double> * getMetricsData();
    const BudgetData * getBudgetData(QString budgetId);
    void setSelectedBudget(QString budgetId);

    // Endpoints
    void getPrediction();
    void getInputs();

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

    // ModelData
    BudgetData * budget;
    // metrics;
    std::vector<double> * metrics; // [wages growth, investment growth, asset growth, years]

    // BudgetModal Boolean
    bool BudgetModal = false;

    Requests * ReqObj;

    QString userid;
};

#endif // CONTROLLER_H
