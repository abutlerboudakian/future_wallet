#include "Controller.h"

Controller::Controller()
{
  PieCreator = new PieGUI;
  BarCreator = new BarGUI;
  LineCreator = new LineGUI;

  metrics = new std::vector<double>;
  metrics->push_back(0);
  metrics->push_back(0);
  metrics->push_back(0);
  metrics->push_back(0);

  budget = new BudgetData;
  budget->addCategory(QString("Alcohol"), 0.05);
  budget->addCategory(QString("Oranges"), 0.55);
  budget->addCategory(QString("Ores"), 0.4);
  Requests * x = new Requests;
  x->getPrediction();
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
  delete budget;
  delete metrics;
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
// Other Methods                      |
// ------------------------------------

/* Function returns the BudgetData in the controller
 * @returns this->budget
 */
const BudgetData * Controller::getBudgetData() const
{
    return this->budget;
}


/* Function returns the std::vector<double> metrics in the controller
 * @returns this->metrics
 */
const std::vector<double> * Controller::getMetricsData() const
{
    return this->metrics;
}

//-------------------------------------
// Endpoints                          |
//-------------------------------------


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
  ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMetrics();
  ((DashBoard*)this->Views->widget(Views::Dashboard))->updateBudget();
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

/* Creates and displays the BudgetPage View as a modal
 */
void Controller::switchToBudgetPage()
{
    if (!BudgetModal)
    {   // Make and show budget modal
        BudgetPage * temp = new BudgetPage;
        temp->setController(this);
        temp->setAttribute(Qt::WA_DeleteOnClose);
        temp->show();
        BudgetModal = true;
    }
}

void Controller::closeBudgetPage()
{
    BudgetModal = false;
}

/* Creates and displays the predictionInputWages View
 * @modifies this->View
 * @effects this->View's top most QWidget is now Views::WagePredict
 */
void Controller::switchToInputWages()
{
  this->Views->setCurrentIndex(Views::WagePredict);
}

/* Creates and displays the predictionInputAssets View
 * @modifies this->View
 * @effects this->View's top most QWidget is now Views::AssetPredict
 */
void Controller::switchToInputAsset()
{
    this->Views->setCurrentIndex(Views::AssetPredict);
}

/* Creates and displays the predictionInputInvest View
 * @modifies this->View
 * @effects this->View's top most QWidget is now Views::InvestPredict
 */
void Controller::switchToInputInvest()
{
    this->Views->setCurrentIndex(Views::InvestPredict);
}

//-------------------------------------
// Charts                             |
//-------------------------------------
/* Creates a pie chart for the given data
 * @requires data to be of the form DataName->PercentDecimal for every pair
 * @param data is a hashmap of std::string->double for DataName->PercentDecimal
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView *Controller::getPieChart(const ChartMap * data)
{
  PieCreator->make(data);
  return PieCreator->getView();
}

/* Creates a bar graph for the given data
 * @requires data to be a ChartMap with std::string->double denoting BarName->Value pairs
 * @param data is a hashmap of std::string->double for BarName->Value
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView * Controller::getBarGraph(const ChartMap * data)
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
QChartView * Controller::getLineGraph(const LineMap * data)
{
  LineCreator->make(data);
  return LineCreator->getView();
}
