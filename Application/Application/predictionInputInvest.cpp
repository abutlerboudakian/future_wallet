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


    connect(ui->Add, SIGNAL(released()), this, SLOT(addStock()));
    connect(ui->Remove, SIGNAL(released()), this, SLOT(removeStock()));

    setupValidator();

}

predictionInputInvest::~predictionInputInvest()
{
    delete ui;
}

// Function to setup validators for all inputs
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
    this->getStockData();
    // Include code to save the input to the controller
    this->controller->switchToInputAsset();
}

// Function to change view to the invest input page
void predictionInputInvest::getWagesView()
{
    // Input saving to controller and validation calls here in ifs
    this->controller->switchToInputWages();
}

// Function to switch view back to the dashboard and save current
// progress in controller
void predictionInputInvest::Exit()
{
    // Add code to tell controller to update it's AssetModel
    this->controller->switchToDashBoard();
}

/* Function used to get all the stock information
 *
 */
void predictionInputInvest::getStockData()
{
    std::cout<<"Start"<<std::endl;
    std::unordered_map<std::string, int> StockData;
    std::cout<<ui->Stocks->count()<<std::endl;
    for (unsigned int i = 0; i < ui->Stocks->count(); i++)
    {
        //QList<QLineEdit*> fields = ((QWidget*)ui->Stocks->itemAt(i))->findChildren<QLineEdit*>(QRegularExpression(QString("/StockData\\d/g")));
        //StockData.insert(std::pair<std::string, int>(fields[0]->text().toStdString(), fields[1]->text().toInt()));
    }
    for (std::unordered_map<std::string, int>::iterator i = StockData.begin(); i != StockData.end(); i++)
    {
        std::cout<<i->first<<" "<<i->second<<std::endl;
    }
    std::cout<<"Done"<<std::endl;
}


// Function modifies the ui to add a new stock field
/* @modifies this->ui
 * @effect this->ui->Stocks has a new stock field
 */
void predictionInputInvest::addStock()
{
    // Constructs a stock field
    QWidget * Stock = new QWidget();
    QHBoxLayout * StockLayout = new QHBoxLayout;
    StockLayout->addWidget(new QLabel("Stock")); // Throw label on the left

    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
    StockLayout->addLayout(Contents);
    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
    QLineEdit * StockName = new QLineEdit, * Shares = new QLineEdit;
    StockName->setObjectName(QString("StockData") + QString::number(counter++));
    Shares->setObjectName(QString("StockData") + QString::number(counter++));
    row1->addWidget(new QLabel("Stock Name"));
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row1->addWidget(StockName);
    row1->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(new QLabel("Shares"));
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    row2->addWidget(Shares);
    row2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(row1);
    Contents->addLayout(row2);

    QHBoxLayout * Buttons = new QHBoxLayout;  // Contains all the buttons
    QPushButton * Add = new QPushButton("Add Stock"), * Remove = new QPushButton("Remove Stock");
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Buttons->addWidget(Add);
    Buttons->addWidget(Remove);
    Buttons->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    Contents->addLayout(Buttons);

    // Adds the stock field
    Stock->setLayout(StockLayout);
    this->ui->Stocks->addWidget(Stock);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addStock()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeStock()));
}

// Function modifies the ui to remove a stock field
/* @modifies this->ui
 * @effect this->ui->Stocks has one less stock field
 *         but has at least one
 */
void predictionInputInvest::removeStock()
{
    if (this->ui->Stocks->count() > 1)
    {
        QPushButton * removeButton = (QPushButton*)sender();
        QWidget * target = removeButton->parentWidget();
        this->ui->Stocks->removeWidget(target);
        delete target;
    }
}

