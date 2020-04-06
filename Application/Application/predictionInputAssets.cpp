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

    setupValidator();
}

predictionInputAssets::~predictionInputAssets()
{
    delete ui;
}

void predictionInputAssets::setupValidator()
{
    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    ui->lineEdit->setValidator(validDouble);
    ui->lineEdit_2->setValidator(validDouble);
    ui->lineEdit_3->setValidator(validDouble);
    ui->lineEdit_4->setValidator(validDouble);
    ui->lineEdit_5->setValidator(validDouble);
    ui->lineEdit_6->setValidator(validDouble);
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
