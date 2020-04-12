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
    connect(ui->RemoveResidence, SIGNAL(released()), this, SLOT(removeRental()));
      
    setupValidator();
}

predictionInputAssets::~predictionInputAssets()
{
    delete ui;
}

// Function to setup validators for all inputs
void predictionInputAssets::setupValidator()
{
    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    /*ui->lineEdit->setValidator(validDouble);
    ui->lineEdit_2->setValidator(validDouble);
    ui->lineEdit_3->setValidator(validDouble);
    ui->lineEdit_4->setValidator(validDouble);
    ui->lineEdit_5->setValidator(validDouble);
    ui->lineEdit_6->setValidator(validDouble);*/
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

void predictionInputAssets::addResidence()
{
//    QWidget * Residence = new QWidget();
//    QHBoxLayout * ResidenceLayout = new QHBoxLayout;
//    ResidenceLayout->addWidget(new QLabel("Residence")); // Throw label on the left

//    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
//    ResidenceLayout->addLayout(Contents);
//    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
//    QLineEdit * ResidenceValue = new QLineEdit, * Location = new QLineEdit;
//    ResidenceValue->setObjectName(QString("ResidenceData") + QString::number(resCounter++));
//    Location->setObjectName(QString("ResidenceData") + QString::number(resCounter++));
//    row1->addWidget(new QLabel("Value"));
//    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    row1->addWidget(ResidenceValue);
//    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    row2->addWidget(new QLabel("Location"));
//    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    row2->addWidget(Location);
//    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    Contents->addLayout(row1);
//    Contents->addLayout(row2);

//    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
//    QPushButton * Add = new QPushButton("Add Residence"), * Remove = new QPushButton("Remove Residence");
//    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    Buttons->addWidget(Add);
//    Buttons->addWidget(Remove);
//    Contents->addLayout(Buttons);

//    // Adds the stock field
//    Residence->setLayout(ResidenceLayout);
//    this->ui->Residences->addWidget(Residence);

//    // Binds the event trigger
//    connect(Add, SIGNAL(released()), this, SLOT(addResidence()));
//    connect(Remove, SIGNAL(released()), this, SLOT(removeResidence()));
}

void predictionInputAssets::addRental()
{
//    QWidget * Rental = new QWidget();
//    QHBoxLayout * RentalLayout = new QHBoxLayout;
//    RentalLayout->addWidget(new QLabel("Rental")); // Throw label on the left

//    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
//    RentalLayout->addLayout(Contents);
//    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
//    QLineEdit * RentalValue = new QLineEdit, * Location = new QLineEdit;
//    RentalValue->setObjectName(QString("RentalData") + QString::number(rentCounter++));
//    Location->setObjectName(QString("RentalData") + QString::number(rentCounter++));
//    row1->addWidget(new QLabel("Value"));
//    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    row1->addWidget(RentalValue);
//    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    row2->addWidget(new QLabel("Location"));
//    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    row2->addWidget(Location);
//    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    Contents->addLayout(row1);
//    Contents->addLayout(row2);

//    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
//    QPushButton * Add = new QPushButton("Add Rental"), * Remove = new QPushButton("Remove Rental");
//    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    Buttons->addWidget(Add);
//    Buttons->addWidget(Remove);
//    Contents->addLayout(Buttons);

//    // Adds the stock field
//    Rental->setLayout(RentalLayout);
//    this->ui->Residences->addWidget(Rental);

//    // Binds the event trigger
//    connect(Add, SIGNAL(released()), this, SLOT(addRental()));
//    connect(Remove, SIGNAL(released()), this, SLOT(removeRental()));
}

void predictionInputAssets::removeResidence()
{
//    if (this->ui->Residences->count() > 1)
//    {
//        QPushButton * removeButton = (QPushButton*)sender();
//        QWidget * target = removeButton->parentWidget();
//        this->ui->Residences->removeWidget(target);
//        delete target;
//    }
}

void predictionInputAssets::removeRental()
{
//    if (this->ui->Rentals->count() > 1)
//    {
//        QPushButton * removeButton = (QPushButton*)sender();
//        QWidget * target = removeButton->parentWidget();
//        this->ui->Rentals->removeWidget(target);
//        delete target;
//    }
}

