#include "Controller.h"

Controller::Controller()
{
  PieCreator = new PieGUI;
  BarCreator = new BarGUI;
  LineCreator = new LineGUI;
}

/* Deletes the chart creators. Delegates deletion of the views to MainApplication
 * @modifies PieCreator, BarCreator, LineCreator
 * @effects Frees PieCreator, BarCreator, LineCreator from the heap
 */
Controller::~Controller()
{
  delete PieCreator;
  delete BarCreator;
  delete LineCreator;
}

/* Sets the views for the controller
 * @requires Views != NULL
 * @param Views is a QStackedWidget containing all the initialized views
 * @modifies this->Views
 * @effects this->Views = Views
 */
void Controller::setViews(QStackedWidget * Views)
{
  this->Views = Views;
}

// ------------------------------------
// View Switching                     |
// ------------------------------------

/* Switches main window view to DashBoard page
 * @modifies this->Views
 * @effects this->View's top most QWidget is now Views::DashBoard
 */
void Controller::switchToDashBoard()
{
  this->Views->setCurrentIndex(Views::Dashboard);
  return;
}

/* Switches main window view to Login page
 * @modifies this->Views
 * @effects this->View's top most QWidget is now Views::Login
 */
void Controller::switchToLogin()
{
  this->Views->setCurrentIndex(Views::Login);
  return;
}

/* Switches main window view to Budgeting page/modal
 * @modifies this->Views
 * @effects this->View's top most QWidget is now Views::BudgetView
 */
void Controller::switchToBudgetPage()
{
    this->Views->setCurrentIndex(Views::BudgetView);
    return;
}

/* Switches main window view to Chart Selection View
 * @modifies this->Views
 * @effects this->View's top most QWidget is now Views::ChartView
 */
void Controller::switchToChartSelection()
{

}

//-------------------------------------
// Charts                             |
//-------------------------------------
/* Creates a pie chart for the given data
 * @requires data to be of the form DataName->PercentDecimal for every pair
 * @param data is a hashmap of std::string->double for DataName->PercentDecimal
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView *Controller::getPieChart(ChartMap * data)
{
  PieCreator->make(data);
  return PieCreator->getView();
}

/* Creates a bar graph for the given data
 * @requires data to be a ChartMap with std::string->double denoting BarName->Value pairs
 * @param data is a hashmap of std::string->double for BarName->Value
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView * Controller::getBarGraph(ChartMap * data)
{
  BarCreator->make(data);
  return BarCreator->getView();
}

/* Creates a line graph for the given data
 * @requires data to be an unorderedmap of std::string->std::vector<std::pair<QDateTime, double> >
 *           where std::string is the line name and std::vector contains points to graph
 *           X Axis is QDateTime, Y is double
 * @param data is an unordered map, representing the lines we need to graph
 * @returns a chartview of the line graph, representing the given data
 */
QChartView * Controller::getLineGraph(LineMap * data)
{
  LineCreator->make(data);
  return LineCreator->getView();
}
