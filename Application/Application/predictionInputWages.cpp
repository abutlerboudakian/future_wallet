#include "predictionInputWages.h"
#include "ui_predictionInputWages.h"

predictionInputWages::predictionInputWages(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::predictionInputWages)
{
    ui->setupUi(this);
    this->controller = controller;

    // Bind slots
    connect(ui->Next, SIGNAL(released()), this, SLOT(getInvestView()));
    connect(ui->Exit, SIGNAL(released()), this, SLOT(Exit()));

    setupValidator();
    // Add industries to the combobox
    ui->Industry->addItems(controller->getIndustries());
}

predictionInputWages::~predictionInputWages()
{
    delete ui;
    delete validDouble;
    delete validInt;
}

/* Function to setup validators for all inputs
 * @modifies: ui->Time (QLineEdit)
 *            ui->Location (QLineEdit)
 *            ui->Amount (QLineEdit)
 * @effects: set validators for all those modified QLineEdits
 *           to meet our application requirements
 */
void predictionInputWages::setupValidator()
{
    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    validInt = new QIntValidator(0, 99, this);
    validInt2 = new QIntValidator(0, 99999, this);
    ui->Time->setValidator(validInt);
    ui->Location->setValidator(validInt2);
    ui->Amount->setValidator(validDouble);
}

/* Convert user inputs on views from simple types to QJsonObject for
 * controller to grab and send to host server to store
 * @returns: a QJsonObject that contains all user inputs from the view
 *           in the structure of
 *           {
 *              “industryCode”: String,
 *              “loc”: String,
 *              “income”: double,
 *              “hourly”: boolean,
 *              “hourspw”: double
 *           }
 */
QJsonObject predictionInputWages::toJSON()
{
    QJsonObject data;

    data.insert("industryCode", ui->Industry->currentText());
    data.insert("loc", ui->Location->text());
    data.insert("income", ui->Amount->text().toDouble());
    QString hourly = "Hourly";
    bool hourly_yes = true;
    if (ui->IncomeType->currentText() == hourly)
    {
        data.insert("hourly", hourly_yes);
    } else {
        hourly_yes = false;
        data.insert("hourly", hourly_yes);
    }
    data.insert("hourspw", ui->Time->text().toDouble());

    return data;
}

/* Convert user inputs on views from simple types to QJsonObject for
 * controller to grab and send to host server to store
 * @requires: savdData is a QJsonObject that is in structure of:
 *            {
 *              “industryCode”: String,
 *              “loc”: String,
 *              “income”: double,
 *              “hourly”: boolean,
 *              “hourspw”: double
 *            }
 * @modifies: ui->Amount (QLineEdit)
 *            ui->Location (QLineEdit)
 *            ui->Time (QLineEdit)
 *            ui->IncomeType (QComboBox)
 *            ui->Industry (QComboBox)
 * @effects: - Populate ui->Amount with the corresponding data from savedData.
 *           - Populate ui->Location with the corresponding data from savedData.
 *           - Populate ui->Time with the corresponding data from savedData.
 *           - Set ui->IncomeType to the corresponding ticker same as in savedData.
 *           - Set ui->Industry to the corresponding ticker same as in savedData.
 */
void predictionInputWages::fromJson(QJsonObject savedData)
{
    // populate Amount
    QJsonObject::Iterator it = savedData.find("income");
    if ( it != savedData.end() )
    {
        ui->Amount->setText(QString::number(int(it.value().toDouble())));
    }

    // populate Location
    it = savedData.find("loc");
    if ( it != savedData.end() )
    {
        ui->Location->setText(it.value().toString());
    }

    // populate Time
    it = savedData.find("hourspw");
    if ( it != savedData.end() )
    {
        ui->Time->setText(QString::number(int(it.value().toDouble())));
    }

    // set IncomeType to the proper index
    it = savedData.find("hourly");
    if ( it != savedData.end() && it.value().toBool() == false )
    {
        int index = ui->IncomeType->findText("Salary");
        ui->IncomeType->setCurrentIndex(index);
    }

    // set Industry to the proper index
    it = savedData.find("industryCode");
    if ( it != savedData.end() )
    {
        int index = ui->Industry->findText(it.value().toString());
        ui->Industry->setCurrentIndex(index);
    }
}

/* Function to clear all user inputs and reset the page to default
 * @modifies: ui->Amount (QLineEdit)
 *            ui->Location (QLineEdit)
 *            ui->Time (QLineEdit)
 *            ui->IncomeType (QComboBox)
 *            ui->Industry (QComboBox)
 * @effects: - set ui->Amount to 0
 *           - set ui->Location to 0
 *           - set ui->Time to 0
 *           - reset ui->IncomeType ticker to index 0
 *           - reset ui->Industry ticker to index 0
 */
void predictionInputWages::clear()
{
    ui->IncomeType->setCurrentIndex(0);
    ui->Industry->setCurrentIndex(0);
    ui->Amount->clear();
    ui->Location->clear();
    ui->Time->clear();
}

//-------------------------------------
// Slots                              |
//-------------------------------------

// Function to change view to the invest input page
void predictionInputWages::getInvestView()
{
    this->controller->switchToInputInvest();
}

// Function to tell controller to send the data to the server
// and return to the dashboard page
void predictionInputWages::Exit()
{
    this->controller->switchToDashBoard();
}
