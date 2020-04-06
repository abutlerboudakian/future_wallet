#include "predictionInputInvest.h"
#include "ui_predictionInputInvest.h"

predictionInputInvest::predictionInputInvest(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::predictionInputInvest)
{
    ui->setupUi(this);
    this->controller = controller;

    connect(ui->Next, SIGNAL(released()), this, SLOT(getAssetView()));
    connect(ui->Back, SIGNAL(released()), this, SLOT(getInvestView()));
    connect(ui->Exit, SIGNAL(released()), this, SLOT(Exit()));
}

predictionInputInvest::~predictionInputInvest()
{
    delete ui;
}

// Function to change view to the asset input page
void predictionInputInvest::getAssetView()
{
    // Include code to save the input to the controller
    this->controller->switchToInputAsset();
}

// Function to change view to the invest input page
void predictionInputInvest::getInvestView()
{
    // Input saving to controller and validation calls here in ifs
    this->controller->switchToInputInvest();
}

// Function to switch view back to the dashboard and save current
// progress in controller
void predictionInputInvest::Exit()
{
    // Add code to tell controller to update it's AssetModel
    this->controller->switchToDashBoard();
}

