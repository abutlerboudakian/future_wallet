#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("future wallet"));

    // _sideMenu = new sideBar(this);
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QGridLayout * gridLay = new QGridLayout(this);
    mainWidget->setLayout(gridLay);
    gridLay->addWidget(new sideBar(this), 0, 0);
    gridLay->addWidget(new QWidget(this), 0, 1, 1, 1);


    this->setMinimumWidth(600);
    this->setMinimumHeight(400);
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete _sideMenu;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // _sideMenu->updateHeight(this);
}
