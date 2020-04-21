#include "Controller.h"

Controller::Controller()
{
  // Set default values
  PieCreator = new PieGUI;
  BarCreator = new BarGUI;
  LineCreator = new LineGUI;

  metrics = nullptr;

  budget = nullptr;

  ReqObj = new Requests;

  years = 0;
}

/* Deletes the chart creators. Delegates deletion of the views to MainApplication
 * @modifies PieCreator, BarCreator, LineCreator, budget, metrics, and ReqObj
 * @effects Frees PieCreator, BarCreator, LineCreator, budget, metrics, and ReqObj from the heap
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

/* Function returns the std::vector<double> metrics in the controller
 * @modifies this->metrics if there is no current metric stored
 * @effect this->metrics contains the most recent prediction metrics
 * @returns this->metrics if there is one set
 *          otherwise, it makes a prediction and returns that metric
 */
const std::vector<double> * Controller::getMetricsData()
{
    if (this->metrics == nullptr)
    {   // Only occurs on startup and nothing is populated yet

        // Get inputs and populate views, then do get prediction
        if (this->getInputs())
        {
            this->getPrediction();
            if (this->metrics->empty())
            {
                QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not retrieve recent metrics from Database. Try again.");
                errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
                errModal->show();
            }
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
/* Function gets the budget from the databse and returns the BudgetData in the controller
 * @param budgetId is a string denoting the associated budget ID
 * @returns this->budget if budgetId=="" else attempts to grab the budget from the
 *                       database and returns it
 * @returns a uniform budget if budgetId = "Uniform"
 */
const BudgetData * Controller::getBudgetData(QString budgetId)
{
    if (budgetId == QString("Uniform"))
    {
        if (this->budget != nullptr)
        {
            delete this->budget;
        }
        this->budget = new BudgetData;
        this->budget->setName(budgetId);
        this->budget->setDollar((*(this->metrics))[0] + (*(this->metrics))[1] + (*(this->metrics))[2]);
        this->budget->addCategory("Savings", 0.5);
        this->budget->addCategory("Spending", 0.5);
    }
    else
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
            this->budget = ReqObj->loadBudget(budgetId, this->userid);
            if (this->budget->isEmpty())
            {
                QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", QString("Could not retrieve budget \"") + budgetId + QString("\". Try again."));
                errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
                errModal->show();
            }
            if (this->budget->getDollar() == 0.0)
            {   // Set dollar amount
                this->budget->setDollar((*(this->metrics))[0] + (*(this->metrics))[1] + (*(this->metrics))[2]);
            }
        }
    }
    return this->budget;
}

/* Function to submit Input to the database and get metrics back
 * @modifies this->metrics this->Views->Dashboard
 * @effect this->metrics now contains the new prediction
 *         the dashboard view updates its metrics in accordance to the prediction returned
 */
void Controller::getPrediction()
{
    if (this->metrics != nullptr)
    {
        delete this->metrics;
    }

    // Get current inputs
    QJsonObject Wages = ((predictionInputWages*)this->Views->widget(Views::WagePredict))->toJSON();
    QJsonObject Invest = ((predictionInputInvest*)this->Views->widget(Views::InvestPredict))->toJSON();
    QJsonObject Assets = ((predictionInputAssets*)this->Views->widget(Views::AssetPredict))->toJSON();
    years = ((predictionInputAssets*)this->Views->widget(Views::AssetPredict))->getYears();

    // Make prediction
    this->metrics = ReqObj->getPrediction(this->userid, Wages, Invest, Assets, years);
    if (this->metrics->empty())
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not make prediction. Try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
        this->metrics->push_back(0);
        this->metrics->push_back(0);
        this->metrics->push_back(0);
        this->metrics->push_back(0);
    }
    ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMetrics();
    this->switchToDashBoard();
}

/* Function to get input from the database and populate the views
 * @modifies this->Views->predictionInputAssets, this->Views->predictionInputInvest, this->Views->predictionInputWages
 * @effect all the aforementioned views are updated with their most recent input data (that was stored in the Database)
 * @throws Error message box on error
 * @returns true if successful, false otherwise
 */
bool Controller::getInputs()
{
    QJsonObject data = ReqObj->getInputs(this->userid);
    if (data.contains("error"))
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not obtain stored input data. Try again after restarting the App.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
        return false;
    }
    else
    {
        ((predictionInputWages*)this->Views->widget(Views::WagePredict))->fromJson(data["wages"].toObject());
        ((predictionInputInvest*)this->Views->widget(Views::InvestPredict))->fromJson(data["invests"].toObject());

        years = (data["years"]).toInt();
        data = data["assets"].toObject();
        data.insert("years", years);

        ((predictionInputAssets*)this->Views->widget(Views::AssetPredict))->fromJson(data);
        return true;
    }
}

/* Function to add a budget to the database
 * @param budget is a BudgetData* of the budget to add to the database
 * @returns true if budget was added, false otherwise on error
 * @returns a MessageBox if an error occurs
 */
bool Controller::addBudget(BudgetData * budget)
{
    if (budget->getName() == QString("") || !ReqObj->addBudget(budget, this->userid))
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not add Budget to Database. Try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
        return false;
    }
    return true;
}

/* Function gets a list of budget names from the ReqObj
 * @returns QStringList of list of names of budgets created by this->Userid
 * @returns an empty QStringList and MessageBox if an error occurs
 */
QStringList Controller::getBudgetList()
{
    std::pair<bool, QStringList> ret = this->ReqObj->listBudgets(this->userid);
    if (!ret.first)
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not get list of user budgets. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
    return ret.second;
}

/* Function to log a user in and do user login data population
 * @param userid is the username the user entered
 * @param Password is the password the user entered
 * @modifies this->userid
 * @effect this->userid = userid on successful login
 */
void Controller::login(QString userid, QString Password)
{
    if (ReqObj->login(userid, Password))
    {
        this->userid = userid;
        ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMessage(userid);
        ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMetrics();
        this->switchToDashBoard();
    }
    else
    { // Failed to Login
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Invalid Credentials. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
}

/* Function logs user out, and goes back to login page */
// @requires user is already logged in
void Controller::logout()
{   
    if (this->Views->currentIndex() != Views::Login)
    {   // Execute if logged in
        ReqObj->logout(this->userid);
        this->userid = QString("");
        delete this->metrics;
        delete this->budget;
        this->metrics = nullptr;
        this->budget = nullptr;
        this->switchToLogin();

        ((predictionInputWages*)this->Views->widget(Views::WagePredict))->clear();
        ((predictionInputAssets*)this->Views->widget(Views::AssetPredict))->clear();
        ((predictionInputInvest*)this->Views->widget(Views::InvestPredict))->clear();
    }
}

/* Function posts registration information to the database
 * @param userid is the username for the new account
 * @param Password is the password for the new account
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
        ((Registration*)this->Views->widget(Views::RegistrationPage))->clear();
        this->switchToLogin();
    }
    else
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Could not create an account. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
        errModal->show();
    }
}

/* Function posts updated user information to the database
 * @param newUserId is the userid to change the current userid to.
 *        it is "" if the userid should not be updated
 * @param Password is the new password. It is "" if the password should not be updated
 * @returns a MessageBox with an error message if error occurs
 *          a MessageBox with success if successfully updated
 */
void Controller::UpdateUserInfo(QString newUserId, QString Password)
{
    if (ReqObj->UpdateUserInfo(this->userid, newUserId, Password))
    {
        if (newUserId != QString(""))
        {
            this->userid = newUserId;
        }
        ((DashBoard*)this->Views->widget(Views::Dashboard))->updateMessage(this->userid);
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
 * @returns a QStringList of all the industries on success
 *          an empty QStringList on error
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
 * @returns a QStringList of all stock tickers on success
 *          an empty QStringList on error
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
        BudgetView = new BudgetPage;
        BudgetView->setController(this);
        BudgetView->setAttribute(Qt::WA_DeleteOnClose);
        BudgetView->updateUserList();
        BudgetView->show();
        BudgetModal = true;
    }
    else
    {
        BudgetView->raise();
    }
}

/* Switches views to the InputBudget view
 * @modifies this->View
 * @effects this->View's top most QWidget is now Views::InputBudget
 */
void Controller::switchToInputBudget(){
    ((InputBudget*)this->Views->widget(Views::BudgetInput))->resetBudget();
    this->Views->setCurrentIndex(Views::BudgetInput);
}

/* Switches to the Registration view
 * @modifies this->View
 * @effects this->View's top most QWidget is now Views::RegistrationPage
 */
void Controller::switchToRegisterPage()
{
    this->Views->setCurrentIndex(Views::RegistrationPage);
}

/* Function to update the state of the budget modal
 * @modifies this->BudgetModal
 * @effect this->BudgetModal = false
 */
void Controller::closeBudgetPage()
{
    BudgetModal = false;
}

/* Creates and displays the AccountManagement View as a modal
 */
void Controller::switchToAccountManage()
{
    if (this->Views->currentIndex() != Views::Login)
    {
        if (!AccountModal)
        {   // Make and show account management modal
            manage = new AccountManagement;
            manage->setController(this);
            manage->setAttribute(Qt::WA_DeleteOnClose);
            manage->show();
            AccountModal = true;
        }
        else
        {
            manage->raise();
        }
    }
}

/* Close AccountManagement modal
 * @modifies this->AccountModal
 * @effect this->AccountModal = false
 */
void Controller::closeAccountManage()
{
    AccountModal = false;
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
 * @param ChartName is the name of the chart
 * @param data is a hashmap of std::string->double for DataName->PercentDecimal
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView *Controller::getPieChart(QString ChartName, const ChartMap * data)
{
  PieCreator->setName(ChartName);
  PieCreator->make(data);
  return PieCreator->getView();
}

/* Creates a bar graph for the given data
 * @requires data to be a ChartMap with std::string->double denoting BarName->Value pairs
 * @param ChartName is the name of the chart
 * @param data is a hashmap of std::string->double for BarName->Value
 * @returns a chartview of the pie chart, representing the given data
 */
QChartView * Controller::getBarGraph(QString ChartName, const ChartMap * data)
{
  BarCreator->setName(ChartName);
  BarCreator->make(data);
  return BarCreator->getView();
}

