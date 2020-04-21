#include "mainapplication.h"
#include "ui_mainapplication.h"

MainApplication::MainApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainApplication)
{
    ui->setupUi(this);

    // Bind slots
    connect(ui->LogOut, SIGNAL(triggered()), this, SLOT(logout()));
    connect(ui->AccountPage, SIGNAL(triggered()), this, SLOT(getUserAccountView()));
}

MainApplication::~MainApplication()
{
    delete ui;
    delete Views;
    delete controller;
}

/* Creates the views for the application, linked to the provided controller
 * @requires maincontroller != NULL
 * @param maincontroller is the Controller to link all the views to
 * @modifies this->Views
 * @effects this->Views now contains every view in the application linked to the maincontroller
 */
void MainApplication::CreateViews(Controller * maincontroller)
{
    Views = new QStackedWidget(this);
    Views->addWidget(new LaunchMenu(this, maincontroller));
    Views->addWidget(new DashBoard(this, maincontroller));
    Views->addWidget(new predictionInputWages(this, maincontroller));
    Views->addWidget(new predictionInputInvest(this, maincontroller));
    Views->addWidget(new predictionInputAssets(this, maincontroller));
    Views->addWidget(new InputBudget(this, maincontroller));
    Views->addWidget(new Registration(this, maincontroller));
    setCentralWidget(Views);
    Views->show();
}

// Function returns the MainApplication's views
QStackedWidget * MainApplication::getViews()
{
    return Views;
}

// Function to set the controller of MainApplication
// @param controller is a Controller
// @modifies this->controller
// @effect this->controller = controller
void MainApplication::setController(Controller * controller)
{
    this->controller = controller;
}

//-------------------------------------
// Slots                              |
//-------------------------------------

// Function switches the user account info page
void MainApplication::getUserAccountView()
{
    this->controller->switchToAccountManage();
}

/* Function Logs the user out */
void MainApplication::logout()
{
    this->controller->logout();
}


/* Function to tell the controller that it is closing
 */
void MainApplication::closeEvent(QCloseEvent * event)
{
    this->controller->logout();
    event->accept();
}
