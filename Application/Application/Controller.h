#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStackedWidget>
#include "ChartTemplate.h"

enum Views
{
    Login = 0,
    Dashboard = 1
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

    // Charts
    QChartView * getPieChart(ChartMap * data);
    QChartView * getBarGraph(ChartMap * data);
    QChartView * getLineGraph(LineMap * data);

  private:
    QStackedWidget * Views;

    // Template Method Creators for the charts
    PieGUI * PieCreator;
    BarGUI * BarCreator;
    LineGUI * LineCreator;
};

#endif // CONTROLLER_H
