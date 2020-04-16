#include "mainapplication.h"
#include "ui_mainapplication.h"

MainApplication::MainApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainApplication)
{
    ui->setupUi(this);

    connect(ui->LogOut, SIGNAL(triggered()), this, SLOT(logout()));
    connect(ui->AccountPage, SIGNAL(triggered()), this, SLOT(getUserAccountView()));

    //hideMenu();
    // Example pie and line graph data
    /*PieCreator = new PieGUI;
    ChartMap * map = new ChartMap;
    map->insert(std::pair<std::string, double>("Cheese", 0.15));
    map->insert(std::pair<std::string, double>("Crust", 0.5));
    map->insert(std::pair<std::string, double>("Sauce", 0.35));
    LineGUI * LineCreator = new LineGUI;
    LineMap * map2 = new LineMap;
    map2->insert(std::pair<std::string, LinePoints>("Orange",
                                                    LinePoints{std::pair<QDateTime, double>(QDateTime(QDate(1999, 1, 1), QTime(0, 5, 0)), 0),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2000, 1, 1), QTime(0, 5, 0)), 10),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2001, 1, 1), QTime(0, 5, 0)), 20)}));
    map2->insert(std::pair<std::string, LinePoints>("Red",
                                                    LinePoints{std::pair<QDateTime, double>(QDateTime(QDate(1999, 1, 1), QTime(0, 50, 0)), 20),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2000, 1, 1), QTime(0, 50, 0)), 6),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2001, 1, 1), QTime(0, 50, 0)), 0)}));
    BarGUI * BarCreator = new BarGUI;
    ChartMap * map3 = new ChartMap;
    map3->insert(std::pair<std::string, double>("Alcohol", 100));
    map3->insert(std::pair<std::string, double>("Oreos", 10.49));
    map3->insert(std::pair<std::string, double>("Oranges", 1000));


    // Initialize both charts
    PieCreator->make(map);
    LineCreator->make(map2);
    BarCreator->make(map3);

    // Add both
    Views->addWidget(BarCreator->getView());
    Views->addWidget(LineCreator->getView());
    Views->addWidget(PieCreator->getView());
    delete map;
    delete map2;
    delete map3;
    delete PieCreator;
    delete LineCreator;
    delete BarCreator;

    //content->addWidget(new LaunchMenu(this, content));
    content->addWidget(new DashBoard(this, content));
    setCentralWidget(content);
    content->show();*/
}

MainApplication::~MainApplication()
{
    delete ui;
    delete Views;
    delete controller;
}

/* Creates the views for the application, linked to the providec controller
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

/* Function sets the username for the menubar
 * @param userId is a QString of the username
 * @modifies the menubar
 * @effect the menubar displays the username now
 */
/*void MainApplication::setUserName(QString userId)
{
    ui->UserName->setTitle(userId);
}

// @modifies menubar
// @effects menubar->setvisible(true);
void MainApplication::showMenu()
{
    //this->menuBar()->setVisible(true);
}

// @modifies menubar
// @effects menubar->setvisible(false);
void MainApplication::hideMenu()
{
    //this->menuBar()->setVisible(false);
}*/


// Function gets the user account info page
void MainApplication::getUserAccountView()
{

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
