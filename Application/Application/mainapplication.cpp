#include "mainapplication.h"
#include "ui_mainapplication.h"

MainApplication::MainApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainApplication)
{
    ui->setupUi(this);

    content = new QStackedWidget(this);
    PieCreator = new PieGUI;
    std::unordered_map<std::string, double> * map = new std::unordered_map<std::string, double>;
    map->insert(std::pair<std::string, double>("Cheese", 0.15));
    map->insert(std::pair<std::string, double>("Crust", 0.5));
    map->insert(std::pair<std::string, double>("Sauce", 0.35));

    PieCreator->initialize(map);
    content->addWidget(PieCreator->getView());
    delete map;

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
}

