#include "Controller.h"

Controller::Controller()
{
  PieCreator = new PieGUI;
  BarCreator = new BarGUI;
  LineCreator = new LineGUI;

  metrics = nullptr;//new std::vector<double>;
  //metrics->push_back(0);
  //metrics->push_back(0);
  //metrics->push_back(0);
  //metrics->push_back(0);

  budget = nullptr;
  /*budget->addCategory(QString("Alcohol"), 0.05);
  budget->addCategory(QString("Oranges"), 0.55);
  budget->addCategory(QString("Ores"), 0.4);*/

  ReqObj = new Requests;
  this->userid = QString("Hello WOrld");
  ReqObj->getIndustries();
  ReqObj->getStocks();
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
  delete ReqObj;
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

/* Function gets the budget from the databse and returns the BudgetData in the controller
 * @param budgetId is a string denoting the associated budget ID
 * @returns this->budget if budgetId=="" else attempts to grab the budget from the
 *                       database and returns it
 */
const BudgetData * Controller::getBudgetData(QString budgetId)
{
    if (budgetId==QString("") && this->budget == nullptr)
    {
        this->budget = new BudgetData;
    }
    else if (budgetId != QString(""))
    {
        if (this->budget != nullptr)
        {
            delete this->budget;
        }
        this->budget = ReqObj->loadBudget(budgetId, "Userid");
        // Include error modal here
    }
    return this->budget;
}


/* Function returns the std::vector<double> metrics in the controller
 * @returns this->metrics
 */
const std::vector<double> * Controller::getMetricsData()
{
    if (this->metrics == nullptr)
    {
        // Get inputs and populate views, then do get prediction
        this->metrics = ReqObj->getPrediction();
        if (this->metrics->empty())
        {
            QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not retrieve recent metrics from Database. Try again.");
            errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
            errModal->show();
        }
    }
    return this->metrics;
}

/* Function gets and sets the budgetid of the Controller and
 * updates the dashboard
 * @param budgetId is a string denoting the associated budgetID
 * @modifies this->budget
 * @effect this->budget is updated to the budget noted by budgetId
 */
void Controller::setSelectedBudget(QString budgetId)
{
    ((DashBoard*)this->Views->widget(Views::Dashboard))->updateBudget(budgetId);
}

//-------------------------------------
// Endpoints                          |
//-------------------------------------
/* Function to submit Input to the database and get metrics back
 * ----------------------------------------------------------------------------------------------------
 * @modifies this->metrics
 * @effect this->metrics now contains the new prediction
 */
void Controller::getPrediction()
{
    if (this->metrics != nullptr)
    {
        delete this->metrics;
    }

    this->metrics = ReqObj->getPrediction();
    if (this->metrics->empty())
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not make prediction. Try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
    ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMetrics();
}

/* Function to get input from the database and populate the views
 */
void Controller::getInputs()
{

}

/* Function to add a budget to the database
 * @param budget is a BudgetData* of the budget to add to the database
 * @returns a MessageBox if an error occurs
 */
void Controller::addBudget(BudgetData * budget)
{
    if (!ReqObj->addBudget(budget, this->userid))
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not add Budget to Database. Try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
}

/* Function gets a list of budget names from the ReqObj
 * @returns QStringList of names of budgets for this->Userid
 * @returns a MessageBox if an error occurs
 */
QStringList Controller::getBudgetList()
{
    QStringList ret = this->ReqObj->listBudgets(this->userid);
    if (ret.isEmpty())
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not get list of user budgets. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
    return ret;
}

void Controller::login(QString userid, QString Password)
{
    // Andrew should implement this

    ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMessage(userid);
    ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMetrics();
    this->switchToDashBoard();
}

/* Function destroys menubar, logs user out, and goes back to login page */
// @requires user is already logged in
void Controller::logout()
{
    // If not logged in
    if (this->Views->currentIndex() != Views::Login)
    {
        //ReqObj->logout(...);
        this->switchToLogin();
        qDebug() << "Hello there";
    }
}

/* Function posts registration information to the database
 * @returns a MessageBox with an error message if error occurs
 *          a MessageBox with success if successfully created account
 */
void Controller::Register(QString userid, QString Password)
{
    if (ReqObj->Register(userid, Password))
    {
        QMessageBox * succModal = new QMessageBox(QMessageBox::NoIcon, "", "Successfully created an account! Please login.");
        succModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        succModal->show();
    }
    else
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not create an account. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
}

/* Function posts updated user information to the database
 * @returns a MessageBox with an error message if error occurs
 *          a MessageBox with success if successfully updated
 */
void Controller::UpdateUserInfo(QString newUserId, QString Password)
{
    if (ReqObj->UpdateUserInfo(this->userid, newUserId, Password))
    {
        QMessageBox * succModal = new QMessageBox(QMessageBox::NoIcon, "", "Successfully updated account information!");
        succModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        succModal->show();
    }
    else
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not update account information. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
}

/* Function requests for a list of all industry names
 * @returns a MessageBox with an error message if error occurs
 *          a MessageBox with success if successfully retrieved
 */
QStringList Controller::getIndustries()
{
    QStringList names = ReqObj->getIndustries();
    if (names.isEmpty())
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not get list of industry names. Please restart the application and try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
    return names;
}

/* Function requests for a list of all stock ticker
 * @returns a MessageBox with an error message if error occurs
 *          a MessageBox with success if successfully retrieved
 */
QStringList Controller::getTickers()
{
    QStringList tickers = ReqObj->getStocks();
    if (tickers.isEmpty())
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not get list of stock tickers. Please restart the application and try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
    return tickers;
}

/* Function to set this->main to the mainapplication
 * @param main is a MainApplication instance
 * @modifies this->main
 * @effect this->main = main
 */
void Controller::setMain(MainApplication * main)
{
    this->main = main;
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
  // ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMetrics();
  // ((DashBoard*)this->Views->widget(Views::Dashboard))->updateBudget(QString(""));
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

/* Creates and displays the BudgetPage View as a modal
 */
void Controller::switchToBudgetPage()
{
    if (!BudgetModal)
    {   // Make and show budget modal
        BudgetPage * temp = new BudgetPage;
        temp->setController(this);
        temp->setAttribute(Qt::WA_DeleteOnClose);
        temp->updateUserList();
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
