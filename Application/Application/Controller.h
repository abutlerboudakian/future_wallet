#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStackedWidget>
#include "ChartTemplate.h"
#include "BudgetData.h"

enum Views
{
    Login = 0,
    Dashboard = 1,
    BudgetView = 2
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
    void switchToChartSelection();

    // Charts
    QChartView * getPieChart(ChartMap * data);
    QChartView * getBarGraph(ChartMap * data);
    QChartView * getLineGraph(LineMap * data);

    // Metric data

  private:
    QStackedWidget * Views;

    // Template Method Creators for the charts
    PieGUI * PieCreator;
    BarGUI * BarCreator;
    LineGUI * LineCreator;

    // Budget Data
    BudgetData * budget;
};

#endif // CONTROLLER_H
