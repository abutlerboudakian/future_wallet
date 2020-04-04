#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStackedWidget>
#include "ChartTemplate.h"
#include "BudgetData.h"

#include "DashBoard.h"
#include "LaunchMenu.h"
#include "BudgetPage.h"

enum Views
{
    Login = 0,
    Dashboard = 1,
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

    void closeBudgetPage();

    // Charts
    QChartView * getPieChart(ChartMap * data);
    QChartView * getBarGraph(ChartMap * data);
    QChartView * getLineGraph(LineMap * data);

    // getMetricData();
    const BudgetData * getBudgetData() const;

  private:
    QStackedWidget * Views;

    // Template Method Creators for the charts
    PieGUI * PieCreator;
    BarGUI * BarCreator;
    LineGUI * LineCreator;

    // ModelData
    BudgetData * budget;
    // metrics;

    // BudgetModal Boolean
    bool BudgetModal = false;
};

#endif // CONTROLLER_H
