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
/* Creates a pie chart for the given data
 * @requires data to be of the form DataName->PercentDecimal for every pair
 * @param data is a hashmap of std::string->double for DataName->PercentDecimal
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView * getPieChart(ChartMap * data)
{
  return PieCreator->make(data);
}

/* Creates a bar graph for the given data
 * @requires data to be a ChartMap with std::string->double denoting BarName->Value pairs
 * @param data is a hashmap of std::string->double for BarName->Value
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView * getBarGraph(ChartMap * data)
{
  return BarCreator->make(data);
}

/* Creates a line graph for the given data
 * @requires data to be an unorderedmap of std::string->std::vector<std::pair<QDateTime, double> >
 *           where std::string is the line name and std::vector contains points to graph
 *           X Axis is QDateTime, Y is double
 * @param data is an unordered map, representing the lines we need to graph
 * @returns a chartview of the line graph, representing the given data
 */
QChartView * getLineGraph(LineMap * data)
{
  return LineCreator->make(data);
}
