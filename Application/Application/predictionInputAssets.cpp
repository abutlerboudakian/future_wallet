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
    connect(ui->AddResidence, SIGNAL(released()), this, SLOT(addResidence()));
    connect(ui->AddRental, SIGNAL(released()), this, SLOT(addRental()));
    connect(ui->RemoveResidence, SIGNAL(released()), this, SLOT(removeResidence()));
    connect(ui->RemoveRental, SIGNAL(released()), this, SLOT(removeRental()));
      
    setupValidator();
}

predictionInputAssets::~predictionInputAssets()
{
    delete ui;
    delete validDouble;
    delete validInt;
}

/* Function to setup validators for all inputs
 * @requires: none
 * @modifies: ui->ResidenceData0 (QLineEdit)
 *            ui->ResidenceData1 (QLineEdit)
 *            ui->RentalData0 (QLineEdit)
 *            ui->RentalData1 (QLineEdit)
 *            ui->Metal (QLineEdit)
 * @effects: set validators for all those modified QLineEdits
 *           to meet our application requirements
 * @returns: none
 */
void predictionInputAssets::setupValidator()
{
    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    validInt = new QIntValidator(0, 99999, this);

    ui->ResidenceData0->setValidator(validDouble);
    ui->ResidenceData1->setValidator(validInt);
    ui->RentalData0->setValidator(validDouble);
    ui->RentalData1->setValidator(validInt);
    ui->Metal->setValidator(validDouble);
}

// Function to submit all the inputs gathered and return to dashboard
void predictionInputAssets::submitInputs()
{
    // add more code to tell the controller to send the data it has to the server and update the model for Assets
    this->controller->getPrediction();
}

// Function to change view to the previous page, the Wages View
// Also save input to the controller
void predictionInputAssets::getInvestView()
{
    // Include code to save input to the controller's models
    this->controller->switchToInputInvest();
}

// Function to change view to the dashboard page
// Also save input to the controller (not post)
void predictionInputAssets::Exit()
{
    // Include code tos ave input to the controller's models
    this->controller->switchToDashBoard();
}


/* Convert user inputs on views from simple types to QJsonObject for
 * controller to grab and send to host server to store
 * @requires: none
 * @modifies: none
 * @effects: none
 * @returns: a QJsonObject that contains all user inputs from the view
 *           in the structure of:
 *           {
 *             “res”: [{“value”: double, “loc”: String}],
 *             “rents”: [{“value”: double, “loc:” String}],
 *             “rm”: double
 *           }
 */
QJsonObject predictionInputAssets::toJSON()
{
    QJsonObject data;

    QJsonArray residences;

    QList<QLineEdit*> list = this->findChildren<QLineEdit *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("ResidenceData*")));
    for (int i = 0; i < 2*ui->Residences->count(); i+=2)
    {
        QJsonObject json_residence;
        json_residence.insert("value", list[i]->text().toDouble());
        json_residence.insert("loc", list[i+1]->text());
        residences.push_back(json_residence);
    }

    data.insert("res", residences);

    QJsonArray rentals;

    list = this->findChildren<QLineEdit *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("RentalData*")));
    for (int i = 0; i < 2*ui->Rentals->count(); i+=2)
    {
        QJsonObject json_rental;
        json_rental.insert("value", list[i]->text().toDouble());
        json_rental.insert("loc", list[i+1]->text());
        rentals.push_back(json_rental);
    }

    data.insert("rents", rentals);

    data.insert("rm", ui->Metal->text().toDouble());

    return data;
}

/* Convert user inputs on views from simple types to QJsonObject for
 * controller to grab and send to host server to store
 * @requires: savdData is a QJsonObject that is in structure of:
 *            {
 *             “res”: [{“value”: double, “loc”: String}],
 *             “rents”: [{“value”: double, “loc:” String}],
 *             “rm”: double
 *            }
 * @modifies: ui->Metal (QLineEdit)
 *            ui->Residences (QVBoxLayout)
 *            ui->Rentals (QVBoxLayout)
 * @effects: - Populate ui->Metal with the corresponding data from savedData.
 *           - Add a QWidget to ui->Residences to represent owned property,
 *             call addResidence( location, value ) to populate view inside the
 *             created QWidget.
 *           - Add a QWidget to ui->Rentals to represent owned property for rent,
 *             call addRental( location, value ) to populate view inside the
 *             created QWidget.
 * @returns: a QJsonObject that contains all user inputs from the view
 */
void predictionInputAssets::fromJson(QJsonObject savedData)
{
    // populate Rare Metal
    QJsonObject::Iterator it = savedData.find("rm");
    if ( it != savedData.end() )
    {
        ui->Metal->setText(QString::number(int(it.value().toDouble())));
    }

    // populat Residences
    it = savedData.find("res");
    if ( it != savedData.end() )
    {
        QJsonArray resList = it.value().toArray();
        QJsonArray::iterator it;

        it = resList.begin();
        if (it != resList.end())
        {
            QJsonObject res = it->toObject();
            QJsonObject::Iterator it_loc = res.find("loc");
            QJsonObject::Iterator it_value = res.find("value");
            if ( it_loc != res.end() && it_value != res.end() )
            {
                QString location = it_loc.value().toString();
                double value = it_value.value().toDouble();
                ui->ResidenceData0->setText(location);
                ui->ResidenceData1->setText(QString::number(value));
            }
            for ( it++; it != resList.end(); it++ )
            {
                QJsonObject res = it->toObject();
                QJsonObject::Iterator it_loc = res.find("loc");
                QJsonObject::Iterator it_value = res.find("value");
                if ( it_loc != res.end() && it_value != res.end() )
                {
                    QString location = it_loc.value().toString();
                    double value = it_value.value().toDouble();
                    addResidence( location, value );
                }
            }
        }
    }

    // populat Rentals
    it = savedData.find("rents");
    if ( it != savedData.end() )
    {
        QJsonArray rentList = it.value().toArray();
        QJsonArray::iterator it;

        it = rentList.begin();
        if (it != rentList.end())
        {
            QJsonObject rent = it->toObject();
            QJsonObject::Iterator it_loc = rent.find("loc");
            QJsonObject::Iterator it_value = rent.find("value");
            if ( it_loc != rent.end() && it_value != rent.end() )
            {
                QString location = it_loc.value().toString();
                double value = it_value.value().toDouble();
                ui->RentalData0->setText(location);
                ui->RentalData1->setText(QString::number(value));
            }
            for ( it++; it != rentList.end(); it++ )
            {
                QJsonObject rent = it->toObject();
                QJsonObject::Iterator it_loc = rent.find("loc");
                QJsonObject::Iterator it_value = rent.find("value");
                if ( it_loc != rent.end() && it_value != rent.end() )
                {
                    QString location = it_loc.value().toString();
                    double value = it_value.value().toDouble();
                    addRental( location, value );
                }
            }
        }
    }
    ui->Years->setText(QString::number(savedData["years"].toInt()));
}

/* Function to clear all user inputs and reset the page to default
 * @requires: none
 * @modifies: ui->Metal (QLineEdit)
 *            ui->Years (QLineEdit)
 *            ui->Residences (QVBoxLayout)
 *            ui->Rentals (QVBoxLayout)
 * @effects: - set ui->Metal to 0
 *           - set ui->Years to 0
 *           - clear and delete all QWidget in ui->Residences,
 *             and create a new one as defualt
 *           - clear and delete all QWidget in ui->Rentals,
 *             and create a new one as defualt
 * @returns: none
 */
void predictionInputAssets::clear()
{
    ui->Metal->clear();
    ui->Years->clear();

    // Delete all created residence widgets, then create a default one
    QLayoutItem *child;
    while( (child = this->ui->Residences->takeAt(0)) != 0 )
    {
        QWidget *target = child->widget();
        this->ui->Residences->removeWidget(target);
        delete target;
    }
    addResidence();

    // Delete all created rental widgets, then create a default one
    while( (child = this->ui->Rentals->takeAt(0)) != 0 )
    {
        QWidget *target = child->widget();
        this->ui->Rentals->removeWidget(target);
        delete target;
    }
    addRental();
}

void predictionInputAssets::addResidence()
{

    QWidget * Residence = new QWidget();
    QHBoxLayout * ResidenceLayout = new QHBoxLayout;
    ResidenceLayout->addWidget(new QLabel("Residence")); // Throw label on the left

    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
    ResidenceLayout->addLayout(Contents);
    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
    QLineEdit * ResidenceValue = new QLineEdit, * Location = new QLineEdit;
    ResidenceValue->setObjectName(QString("ResidenceData") + QString::number(resCounter++));
    Location->setObjectName(QString("ResidenceData") + QString::number(resCounter++));
    row1->addWidget(new QLabel("Value"));
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row1->addWidget(ResidenceValue);
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(new QLabel("Location"));
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(Location);
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(row1);
    Contents->addLayout(row2);

    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
    QPushButton * Add = new QPushButton("Add Residence"), * Remove = new QPushButton("Remove Residence");
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Buttons->addWidget(Add);
    Buttons->addWidget(Remove);
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(Buttons);

    // SetValidator for residence value and location
    ResidenceValue->setValidator(validDouble);
    Location->setValidator(validInt);

    // Adds the residences field
    Residence->setLayout(ResidenceLayout);
    this->ui->Residences->addWidget(Residence);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addResidence()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeResidence()));

}

void predictionInputAssets::addResidence(QString location, double value)
{

    QWidget * Residence = new QWidget();
    QHBoxLayout * ResidenceLayout = new QHBoxLayout;
    ResidenceLayout->addWidget(new QLabel("Residence")); // Throw label on the left

    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
    ResidenceLayout->addLayout(Contents);
    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
    QLineEdit * ResidenceValue = new QLineEdit, * Location = new QLineEdit;
    ResidenceValue->setObjectName(QString("ResidenceData") + QString::number(resCounter++));
    Location->setObjectName(QString("ResidenceData") + QString::number(resCounter++));
    row1->addWidget(new QLabel("Value"));
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row1->addWidget(ResidenceValue);
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(new QLabel("Location"));
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(Location);
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(row1);
    Contents->addLayout(row2);

    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
    QPushButton * Add = new QPushButton("Add Residence"), * Remove = new QPushButton("Remove Residence");
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Buttons->addWidget(Add);
    Buttons->addWidget(Remove);
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(Buttons);

    // SetValidator for residence value and location
    ResidenceValue->setValidator(validDouble);
    Location->setValidator(validInt);

    // Set default value for stock name and shares
    Location->setText(location);
    QString str_value = QString::number(value);
    ResidenceValue->setText(str_value);

    // Adds the residences field
    Residence->setLayout(ResidenceLayout);
    this->ui->Residences->addWidget(Residence);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addResidence()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeResidence()));

}

void predictionInputAssets::addRental()
{
    QWidget * Rental = new QWidget();
    QHBoxLayout * RentalLayout = new QHBoxLayout;
    RentalLayout->addWidget(new QLabel("Rental")); // Throw label on the left

    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
    RentalLayout->addLayout(Contents);
    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
    QLineEdit * RentalValue = new QLineEdit, * Location = new QLineEdit;
    RentalValue->setObjectName(QString("RentalData") + QString::number(rentCounter++));
    Location->setObjectName(QString("RentalData") + QString::number(rentCounter++));
    row1->addWidget(new QLabel("Value"));
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row1->addWidget(RentalValue);
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(new QLabel("Location"));
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(Location);
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(row1);
    Contents->addLayout(row2);

    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
    QPushButton * Add = new QPushButton("Add Rental"), * Remove = new QPushButton("Remove Rental");
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Buttons->addWidget(Add);
    Buttons->addWidget(Remove);
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(Buttons);

    // SetValidator for rental value and location
    RentalValue->setValidator(validDouble);
    Location->setValidator(validInt);

    // Adds the rentals field
    Rental->setLayout(RentalLayout);
    this->ui->Rentals->addWidget(Rental);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addRental()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeRental()));
}

void predictionInputAssets::addRental(QString location, double value)
{
    QWidget * Rental = new QWidget();
    QHBoxLayout * RentalLayout = new QHBoxLayout;
    RentalLayout->addWidget(new QLabel("Rental")); // Throw label on the left

    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
    RentalLayout->addLayout(Contents);
    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
    QLineEdit * RentalValue = new QLineEdit, * Location = new QLineEdit;
    RentalValue->setObjectName(QString("RentalData") + QString::number(rentCounter++));
    Location->setObjectName(QString("RentalData") + QString::number(rentCounter++));
    row1->addWidget(new QLabel("Value"));
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row1->addWidget(RentalValue);
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(new QLabel("Location"));
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(Location);
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(row1);
    Contents->addLayout(row2);

    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
    QPushButton * Add = new QPushButton("Add Rental"), * Remove = new QPushButton("Remove Rental");
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Buttons->addWidget(Add);
    Buttons->addWidget(Remove);
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(Buttons);

    // SetValidator for rental value and location
    RentalValue->setValidator(validDouble);
    Location->setValidator(validInt);

    // Set default value for stock name and shares
    Location->setText(location);
    QString str_value = QString::number(value);
    RentalValue->setText(str_value);

    // Adds the rentals field
    Rental->setLayout(RentalLayout);
    this->ui->Rentals->addWidget(Rental);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addRental()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeRental()));
}

void predictionInputAssets::removeResidence()
{
    if (this->ui->Residences->count() > 1)
    {
        QPushButton * removeButton = (QPushButton*)sender();
        QWidget * target = removeButton->parentWidget();
        this->ui->Residences->removeWidget(target);
        delete target;
    }
}

void predictionInputAssets::removeRental()
{
    if (this->ui->Rentals->count() > 1)
    {
        QPushButton * removeButton = (QPushButton*)sender();
        QWidget * target = removeButton->parentWidget();
        this->ui->Rentals->removeWidget(target);
        delete target;
    }
}

/* Function to get the number of years to predict with
 * @returns ui->Years;
 */
int predictionInputAssets::getYears()
{
    return ui->Years->text().toInt();
}
