#include "predictionInputAssets.h"
#include "ui_predictionInputAssets.h"

predictionInputAssets::predictionInputAssets(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::predictionInputAssets)
{
    ui->setupUi(this);

    this->controller = controller;

    connect(ui->Submit, SIGNAL(released()), this, SLOT(submitInputs()));
    connect(ui->Back, SIGNAL(released()), this, SLOT(getWagesView()));
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

// Function to change view to the previous page, the Wages View
// Also save input to the controller
void predictionInputAssets::getWagesView()
{
    // Include code to save input to the controller's models
    this->controller->switchToInputWages();
}

// Function to change view to the dashboard page
// Also save input to the controller (not post)
void predictionInputAssets::Exit()
{
    // Include code tos ave input to the controller's models
    this->controller->switchToDashBoard();
}
