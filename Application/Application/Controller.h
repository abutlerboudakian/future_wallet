#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStackedWidget>
#include "ChartTemplate.h"

class Controller
{
  public:
    Controller(QStackedWidget * Views);
    ~Controller();

    void switchToDashBoard();
    void switchToLogin();

    QChartView * 

  private:

};

#endif // CONTROLLER_H
