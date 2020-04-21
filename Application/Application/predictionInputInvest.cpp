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
    stockList = controller->getTickers();
    stockList.prepend("--");
    ui->StockData0->addItems(stockList);
}

predictionInputInvest::~predictionInputInvest()
{
    delete ui;
    delete validDouble;
}

/* Function to setup validators for all inputs
 * @requires: none
 * @modifies: ui->Saving (QLineEdit)
 *            ui->CD (QLineEdit)
 *            ui->StockData1 (QLineEdit)
 *            ui->Bond (QLineEdit)
 *            ui->TBond (QLineEdit)
 * @effects: set validators for all those modified QLineEdits
 *           to meet our application requirements
 * @returns: none
 */
void predictionInputInvest::setupValidator()
{

    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);
    ui->Saving->setValidator(validDouble);
    ui->CD->setValidator(validDouble);
    ui->StockData1->setValidator(validDouble);
    ui->Bond->setValidator(validDouble);
    ui->TBond->setValidator(validDouble);
}

// Function to change view to the asset input page
void predictionInputInvest::getAssetView()
{
    //this->getStockData();
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

/* Convert user inputs on views from simple types to QJsonObject for
 * controller to grab and send to host server to store
 * @requires: none
 * @modifies: none
 * @effects: none
 * @returns: a QJsonObject that contains all user inputs from the view
 *           in the structure of:
 *           {
 *             “savings”: double,
 *             “cd”: double,
 *             “stocks”: [{ticker: shares}],
 *             “bonds”: double,
 *             “tbonds”: double
 *           }
 */
QJsonObject predictionInputInvest::toJSON()
{
    QJsonObject data;

    data.insert("savings", ui->Saving->text().toDouble());
    data.insert("cd", ui->CD->text().toDouble());

    QJsonObject stocks;

    QString StockName;
    QList<QComboBox*> names = this->findChildren<QComboBox *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("StockData*")));
    QList<QLineEdit*> shares = this->findChildren<QLineEdit *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("StockData*")));
    for (int i = 0; i < ui->Stocks->count(); i++)
    {
        StockName = names[i]->itemText(names[i]->currentIndex());
        if (StockName != QString("--"))
        {
            stocks.insert(StockName, shares[i]->text().toInt());
        }
    }

    data.insert("stocks", stocks);

    data.insert("bonds", ui->Bond->text().toDouble());
    data.insert("tbonds", ui->TBond->text().toDouble());

    return data;
}

/* Convert user inputs on views from simple types to QJsonObject for
 * controller to grab and send to host server to store
 * @requires: savdData is a QJsonObject that is in structure of:
 *            {
 *              “savings”: double,
 *              “cd”: double,
 *              “stocks”: [{ticker: shares}],
 *              “bonds”: double,
 *              “tbonds”: double
 *            }
 * @modifies: ui->Saving (QLineEdit)
 *            ui->CD (QLineEdit)
 *            ui->Bond (QLineEdit)
 *            ui->TBond (QLineEdit)
 *            ui->Stocks (QVBoxLayout)
 * @effects: - Populate ui->Saving with the corresponding data from savedData.
 *           - Populate ui->CD with the corresponding data from savedData.
 *           - Populate ui->Bond with the corresponding data from savedData.
 *           - Populate ui->TBond with the corresponding data from savedData.
 *           - Add a QWidget to ui->Stocks to represent owned stocks, call
 *             addStock( name, value ) to populate view inside the created QWidget.
 * @returns: none
 */
void predictionInputInvest::fromJson(QJsonObject savedData)
{
    // populate Saving
    QJsonObject::Iterator it = savedData.find("savings");
    if ( it != savedData.end() )
    {
        ui->Saving->setText(QString::number(int(it.value().toDouble())));
    }

    // populate CD
    it = savedData.find("cd");
    if ( it != savedData.end() )
    {
        ui->CD->setText(QString::number(int(it.value().toDouble())));
    }

    // populate Bond
    it = savedData.find("bonds");
    if ( it != savedData.end() )
    {
        ui->Bond->setText(QString::number(int(it.value().toDouble())));
    }

    // populate Treasury Bond
    it = savedData.find("tbonds");
    if ( it != savedData.end() )
    {
        ui->TBond->setText(QString::number(int(it.value().toDouble())));
    }

    // populat Stocks
    it = savedData.find("stocks");
    if ( it != savedData.end() )
    {
        QJsonObject stocksList = it.value().toObject();
        it = stocksList.begin();
        if (it != stocksList.end())
        {
            QString name = it.key();
            double shares = it.value().toDouble();
            addStock(name, shares);
            for ( it++; it != stocksList.end(); it++ )
            {
                QString name = it.key();
                double shares = it.value().toDouble();
                addStock(name, shares);
            }
        }
    }
}

/* Function to clear all user inputs and reset the page to default
 * @requires: none
 * @modifies: ui->Saving (QLineEdit)
 *            ui->CD (QLineEdit)
 *            ui->Bond (QLineEdit)
 *            ui->TBond (QLineEdit)
 *            ui->Stocks (QVBoxLayout)
 * @effects: - set ui->Saving to 0
 *           - set ui->CD to 0
 *           - set ui->Bond to 0
 *           - set ui->TBond to 0
 *           - clear and delete all QWidget in ui->Stocks,
 *             and create a new one as defualt
 * @returns: none
 */
void predictionInputInvest::clear()
{
    ui->Saving->clear();
    ui->CD->clear();
    ui->Bond->clear();
    ui->TBond->clear();

    // Delete all created stock widgets
    QLayoutItem *child;
    while( (child = this->ui->Stocks->takeAt(0)) != 0 )
    {
        QWidget *target = child->widget();
        this->ui->Stocks->removeWidget(target);
        delete target;
    }
    addStock();
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
    QLineEdit * Shares = new QLineEdit;
    QComboBox * StockName = new QComboBox;
    StockName->addItems(stockList);
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

    // SetValidator for shares
    Shares->setValidator(validDouble);

    // Adds the stock field
    Stock->setLayout(StockLayout);
    this->ui->Stocks->addWidget(Stock);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addStock()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeStock()));
}

// Function modifies the ui to add a new stock field
/* @modifies this->ui
 * @effect this->ui->Stocks has a new stock field
 */
void predictionInputInvest::addStock(QString name, double shares)
{
    // Constructs a stock field
    QWidget * Stock = new QWidget();
    QHBoxLayout * StockLayout = new QHBoxLayout;
    StockLayout->addWidget(new QLabel("Stock")); // Throw label on the left

    QVBoxLayout * Contents = new QVBoxLayout; // Contents of the stock
    StockLayout->addLayout(Contents);
    QHBoxLayout * row1 = new QHBoxLayout, *row2 = new QHBoxLayout;
    QLineEdit * Shares = new QLineEdit;
    QComboBox * StockName = new QComboBox;
    StockName->addItems(stockList);
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

    // Set Validator for shares
    Shares->setValidator(validDouble);

    // Set default value for stock name and shares
    int index = StockName->findData(name);
    StockName->setCurrentIndex((index < 0 ? 0 : index));
    QString str_shares = QString::number(shares);
    Shares->setText(str_shares);

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

