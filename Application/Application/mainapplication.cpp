#include "mainapplication.h"
#include "ui_mainapplication.h"

MainApplication::MainApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainApplication)
{
    ui->setupUi(this);

    content = new QStackedWidget(this);

    // Example pie and line graph data
    PieCreator = new PieGUI;
    ChartMap * map = new std::unordered_map<std::string, double>;
    map->insert(std::pair<std::string, double>("Cheese", 0.15));
    map->insert(std::pair<std::string, double>("Crust", 0.5));
    map->insert(std::pair<std::string, double>("Sauce", 0.35));
    LineCreator = new LineGUI;
    LineMap * map2 = new LineMap;
    map2->insert(std::pair<std::string, LinePoints>("Orange",
                                                    LinePoints{std::pair<QDateTime, double>(QDateTime(QDate(1999, 1, 1), QTime(0, 5, 0)), 0),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2000, 1, 1), QTime(0, 5, 0)), 10),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2001, 1, 1), QTime(0, 5, 0)), 20)}));
    map2->insert(std::pair<std::string, LinePoints>("Red",
                                                    LinePoints{std::pair<QDateTime, double>(QDateTime(QDate(1999, 1, 1), QTime(0, 50, 0)), 20),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2000, 1, 1), QTime(0, 50, 0)), 6),
                                                               std::pair<QDateTime, double>(QDateTime(QDate(2001, 1, 1), QTime(0, 50, 0)), 0)}));

    // Initialize both charts
    PieCreator->initialize(map);
    LineCreator->initialize(map2);

    // Add both
    content->addWidget(LineCreator->getView());
    content->addWidget(PieCreator->getView());
    delete map;
    delete map2;

    content->addWidget(new LaunchMenu(this, content));
    content->addWidget(new DashBoard(this, content));
    setCentralWidget(content);
    content->show();
}

MainApplication::~MainApplication()
{
    delete ui;
    delete content;
    delete PieCreator;
    delete LineCreator;
}

