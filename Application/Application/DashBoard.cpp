#include "DashBoard.h"
#include "ui_DashBoard.h"

DashBoard::DashBoard(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::DashBoard),
    BaseView()
{
    ui->setupUi(this);
    this->controller = controller;
    // Set up event triggers:

    connect(ui->Predict, SIGNAL(released()), this, SLOT(getInputView()));
    connect(ui->Budget, SIGNAL(released()), this, SLOT(getBudgetView()));
    connect(ui->Pie, SIGNAL(released()), this, SLOT(getPieView()));
    connect(ui->Bar, SIGNAL(released()), this, SLOT(getVBarView()));
    connect(ui->Line, SIGNAL(released()), this, SLOT(getLineView()));
}

// Delegate destruction of the other elements to the base QWidget destructor
DashBoard::~DashBoard()
{
    delete ui;
}

/* Function used to update the metric in display
 * @modifies this->ui->metrics
 * @effects this->ui->metrics now contains a new treewidget, representing the new metric
 */
void DashBoard::update()
{
    // Ask controller for metric data

    // Remove current tree widget
    delete ui->metrics->takeWidget();

    QTreeWidget * mWidget = new QTreeWidget; // Might need to specify parent
    mWidget->setColumnCount(2);

    // Wages root
    QTreeWidgetItem * root = new QTreeWidgetItem(mWidget), * tmpItem;
    root->setText(0, "Wages");
    root->setText(1, INSERTTOTALWAGES);
    mWidget->addTopLevelItem(root);
    // Input each wage component and value
    for each component
    {
        tmpItem = new QTreeWidgetItem(root);
        tmpItem->setText(0, COMPONENT NAME);
        tmpItem->setText(1, COMPONENT VALUE);
        root->addChild(tmpItem);
    }


    ui->metrics->setWidget(mWidget);
}


// Slots -- aka signals

// Function to switch to the input view when "Predict" button is pressed
void DashBoard::getInputView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to start the Budget Modal when the "Budget" button is pressed
void DashBoard::getBudgetView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to show the metric as a pie chart
void DashBoard::getPieView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to show the metric as a vertical bar graph
void DashBoard::getVBarView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to show the metric as a line graph
void DashBoard::getLineView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}
