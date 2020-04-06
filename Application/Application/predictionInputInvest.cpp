#include "predictionInputInvest.h"
#include "ui_predictionInputInvest.h"

predictionInputInvest::predictionInputInvest(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::predictionInputInvest)
{
    ui->setupUi(this);
    this->controller = controller;

    connect(ui->Next, SIGNAL(released()), this, SLOT(getAssetView()));
    connect(ui->Back, SIGNAL(released()), this, SLOT(getWagesView()));
    connect(ui->Exit, SIGNAL(released()), this, SLOT(Exit()));

    setupValidator();
}

predictionInputInvest::~predictionInputInvest()
{
    delete ui;
}

void predictionInputInvest::setupValidator()
{
    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    validInt = new QIntValidator(10000, 99999, this);
    ui->lineEdit->setValidator(validInt);
    ui->lineEdit_2->setValidator(validDouble);
    ui->lineEdit_3->setValidator(validDouble);
    ui->lineEdit_4->setValidator(validInt);
    ui->lineEdit_6->setValidator(validDouble);
}

// Function to change view to the asset input page
void predictionInputInvest::getAssetView()
{
    // Include code to save the input to the controller
    this->controller->switchToInputAsset();
}

// Function to change view to the previous page, the Wages View
// Also save input to the controller
void predictionInputInvest::getWagesView()
{
    // Include code to save input to the controller's models
    this->controller->switchToInputWages();
}

// Function to change view to the dashboard page
// Also save input to the controller (not post)
void predictionInputInvest::Exit()
{
    // Include code tos ave input to the controller's models
    this->controller->switchToDashBoard();
}
