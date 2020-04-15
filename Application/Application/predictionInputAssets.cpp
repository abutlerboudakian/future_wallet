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

// Function to setup validators for all inputs
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
    this->controller->switchToDashBoard(); // Perhaps move this to controller
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

void predictionInputAssets::getResidenceData()
{
//    std::cout<<"Start"<<std::endl;
//    std::unordered_map<std::string, int> ResidenceData;
//    QObjectList Residences = ui->Residences->children();
//    for (QObjectList::const_iterator i = Residences.begin(); i != Residences.end(); i++)
//    {
//        QList<QLineEdit*> fields = ((QWidget*)*i)->findChildren<QLineEdit*>(QRegularExpression("ResidenceData"));
//        ResidenceData.insert(std::pair<std::string, int>(fields[0]->text().toStdString(), fields[1]->text().toInt()));
//    }
//    for (std::unordered_map<std::string, int>::iterator i = ResidenceData.begin(); i != ResidenceData.end(); i++)
//    {
//        std::cout<<i->first<<" "<<i->second<<std::endl;
//    }
//    std::cout<<"Done"<<std::endl;
}

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

void predictionInputAssets::fromJson(QJsonObject savedData)
{
    // populate Rare Metal
    QJsonObject::Iterator it = savedData.find("rm");
    if ( it != savedData.end() )
    {
        ui->Metal->setText(it.value().toString());
    }

    // populat Residences
    it = savedData.find("res");
    if ( it != savedData.end() )
    {
        QJsonArray resList = it.value().toArray();
        QJsonArray::iterator it;
        for ( it = resList.begin(); it != resList.end(); it++ )
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

    // populat Rentals
    it = savedData.find("rents");
    if ( it != savedData.end() )
    {
        QJsonArray rentList = it.value().toArray();
        QJsonArray::iterator it;
        for ( it = rentList.begin(); it != rentList.end(); it++ )
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

