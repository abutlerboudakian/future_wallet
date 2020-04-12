#include "predictionInputWages.h"
#include "ui_predictionInputWages.h"

predictionInputWages::predictionInputWages(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::predictionInputWages)
{
    ui->setupUi(this);
    this->controller = controller;

    connect(ui->Next, SIGNAL(released()), this, SLOT(getInvestView()));
    connect(ui->Exit, SIGNAL(released()), this, SLOT(Exit()));

    setupValidator();
}

predictionInputWages::~predictionInputWages()
{
    delete ui;
    delete validDouble;
    delete validInt;
}

// Function to setup validators for all inputs
void predictionInputWages::setupValidator()
{
    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    validInt = new QIntValidator(0, 99, this);
    validInt2 = new QIntValidator(0, 99999, this);
    ui->Time->setValidator(validInt);
    ui->Location->setValidator(validInt2);
    ui->Amount->setValidator(validDouble);
}

// Slots

// Function to change view to the invest input page
void predictionInputWages::getInvestView()
{
    // Input saving to controller and validation calls here in ifs
    this->controller->switchToInputInvest();
}

// Function to tell controller to send the data to the server
// and return to the dashboard page
void predictionInputWages::Exit()
{
    // Add code to save input to the controller's model
    this->controller->switchToDashBoard();
}
