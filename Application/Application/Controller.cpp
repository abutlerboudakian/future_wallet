#include "Controller.h"

Controller::Controller(QStackedWidget * Views)
{
  this->Views = Views;
  PieCreator = new PieGUI;
  BarCreator = new BarGUI:
  LineCreator = new LineGUI;
}

/* Deletes the Views
 * @modifies this->Views
 * @effects frees the views stored in this->Views
 */
Controller::~Controller()
{
  delete Views;
  delete PieCreator;
  delete BarCreator;
  delete LineCreator;
}


// ------------------------------------
// View Switching                     |
// ------------------------------------

/* Switches main window view to DashBoard page
 * @modifies this->Views
 * @effects this->View's top most QWidget is now Views::DashBoard
 */
void switchToDashBoard()
{
  this->Views->setCurrentIndex(Views::DashBoard);
  return;
}

/* Switches main window view to Login page
 * @modifies this->Views
 * @effects this->View's top most QWidget is now Views::Login
 */
void switchToLogin()
{
  this->Views->setCurrentIndex(Views::Login);
  return;
}


//-------------------------------------
// Charts                             |
//-------------------------------------
QChartView * getPieChart(ChartMap * data);
QChartView * getBarGraph(ChartMap * data);
QChartView * getLineGraph(LineMap * data);
