#include "predictionInputAssets.h"
#include "ui_predictionInputAssets.h"

predictionInputAssets::predictionInputAssets(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::predictionInputAssets)
{
    ui->setupUi(this);

    this->controller = controller;

    connect(ui->Submit, SIGNAL(released()), this, SLOT(submitInputs()));
    connect(ui->Back, SIGNAL(released()), this, SLOT(getInvestView()));
    connect(ui->Exit, SIGNAL(released()), this, SLOT(Exit()));
}

predictionInputAssets::~predictionInputAssets()
{
    delete ui;
}

// Function to submit all the inputs gathered and return to dashboard
void predictionInputAssets::submitInputs()
{
    // add more code to tell the controller to send the data it has to the server and update the model for Assets
    this->controller->switchToDashBoard(); // Perhaps move this to controller
}

// Function to change view to the invest input page
void predictionInputAssets::getInvestView()
{
    // Input saving to controller and validation calls here in ifs
    this->controller->switchToInputInvest();
}

// Function to switch view back to the dashboard and save current
// progress in controller
void predictionInputAssets::Exit()
{
    // Add code to tell controller to update it's AssetModel
    this->controller->switchToDashBoard();
}
