#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnPieChart, SIGNAL(released()), this, SLOT(btnPieChartClicked()));
    connect(ui->btnChangeGraph, SIGNAL(released()), this, SLOT(btnChangeGraphClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void btnPieChartClicked()
{
}

void btnChangeGraphClicked()
{
}

