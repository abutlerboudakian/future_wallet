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
    ui->StockData0->addItems(stockList);
}

predictionInputInvest::~predictionInputInvest()
{
    delete ui;
    delete validDouble;
}

// Function to setup validators for all inputs
void predictionInputInvest::setupValidator()
{

    validDouble = new QDoubleValidator(0.00, 99999999.00, 2);    ui->Saving->setValidator(validDouble);
    ui->CD->setValidator(validDouble);
    //ui->StockData0->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9]{0,5}") , this ));
    ui->StockData1->setValidator(validDouble);
    ui->Mutual->setValidator(validDouble);
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

/* Function used to get all the stock information
 *
 */
/*void predictionInputInvest::getStockData()
{
    std::cout<<"Start"<<std::endl;
    std::unordered_map<std::string, int> StockData;
    std::cout<<ui->Stocks->count()<<std::endl;*/
    /*for (unsigned int i = 0; i < ui->Stocks->count(); i++)
    {

        QList<QLineEdit*> fields = ((QWidget*)ui->Stocks->itemAt(i))->findChildren<QLineEdit*>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("StockData*")));
        StockData.insert(std::pair<std::string, int>(fields[0]->text().toStdString(), fields[1]->text().toInt()));
    }*/
 /*   QList<QLineEdit*> list = this->findChildren<QLineEdit *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("StockData*"))); // Because its children of this, not children of a layout
    for (unsigned int i = 0; i < 2*ui->Stocks->count(); i+=2)
    {
        StockData.insert(std::pair<std::string, int>(list[i]->text().toStdString(), list[i+1]->text().toInt()));
    }
    for (std::unordered_map<std::string, int>::iterator i = StockData.begin(); i != StockData.end(); i++)
    {
        std::cout<<i->first<<" "<<i->second<<std::endl;
    }
    std::cout<<"Done"<<std::endl;
}*/

QJsonObject predictionInputInvest::toJSON()
{
    QJsonObject data;

    data.insert("savings", ui->Saving->text().toDouble());
    data.insert("cd", ui->CD->text().toDouble());

    QJsonArray stocks;

    QList<QComboBox*> names = this->findChildren<QComboBox *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("StockData*")));
    QList<QLineEdit*> shares = this->findChildren<QLineEdit *>(QRegularExpression(QRegularExpression::wildcardToRegularExpression("StockData*")));
    for (int i = 0; i < ui->Stocks->count(); i++)
    {
        QJsonObject json_stock;
        qDebug() << names[i]->itemText(names[i]->currentIndex());
        json_stock.insert(names[i]->itemText(names[i]->currentIndex()), shares[i]->text().toDouble());
        stocks.push_back(json_stock);
    }

    data.insert("stocks", stocks);

    data.insert("bonds", ui->Bond->text().toDouble());
    data.insert("tbonds", ui->TBond->text().toDouble());

    return data;
}

void predictionInputInvest::fromJson(QJsonObject savedData)
{
    // populate Saving
    QJsonObject::Iterator it = savedData.find("savings");
    if ( it != savedData.end() )
    {
        ui->Saving->setText(it.value().toString());
    }

    // populate CD
    it = savedData.find("cd");
    if ( it != savedData.end() )
    {
        ui->CD->setText(it.value().toString());
    }

    // populate Bond
    it = savedData.find("bonds");
    if ( it != savedData.end() )
    {
        ui->Bond->setText(it.value().toString());
    }

    // populate Treasury Bond
    it = savedData.find("tbonds");
    if ( it != savedData.end() )
    {
        ui->TBond->setText(it.value().toString());
    }

    // populat Stocks
    it = savedData.find("stocks");
    if ( it != savedData.end() )
    {
        QJsonArray stocksList = it.value().toArray();
        QJsonArray::iterator it;
        for ( it = stocksList.begin(); it != stocksList.end(); it++ )
        {
            QJsonObject stock = it->toObject();
            if ( stock.empty() == false )
            {
                QString name = stock.begin()->toString();
                double shares = stock.begin()->toDouble();
                addStock(name, shares);
            }
        }
    }
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
    //StockName->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9]{0,5}") , this ));

    // Adds the stock field
    Stock->setLayout(StockLayout);
    this->ui->Stocks->addWidget(Stock);

    // Binds the event trigger
    connect(Add, SIGNAL(released()), this, SLOT(addStock()));
    connect(Remove, SIGNAL(released()), this, SLOT(removeStock()));
}

void predictionInputInvest::addStock(QString name, double shares)
{
    qDebug()<<"Other";
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
    //StockName->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9]{0,5}") , this ));

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

