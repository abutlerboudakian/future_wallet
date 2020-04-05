#include "DashBoard.h"
#include "ui_DashBoard.h"

DashBoard::DashBoard(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::DashBoard)
{
    ui->setupUi(this);
    this->controller = controller;
    // Set up event triggers:

    connect(ui->Predict, SIGNAL(released()), this, SLOT(getInputView()));
    connect(ui->Budget, SIGNAL(released()), this, SLOT(getBudgetView()));
    connect(ui->metricsPie, SIGNAL(released()), this, SLOT(getMetricsPieView()));
    connect(ui->metricsBar, SIGNAL(released()), this, SLOT(getMetricsBarView()));
    connect(ui->metricsLine, SIGNAL(released()), this, SLOT(getMetricsLineView()));
    connect(ui->budgetPie, SIGNAL(released()), this, SLOT(getBudgetPieView()));
    connect(ui->budgetBar, SIGNAL(released()), this, SLOT(getBudgetBarView()));
}

// Delegate destruction of the other elements to the base QWidget destructor
DashBoard::~DashBoard()
{
    delete ui;
}

/* Function used to update the budget in display
 * @modifies this->ui->Budget
 * @effects this->ui->Budget now contains the budget that was recently loaded in
 */
void DashBoard::updateBudget()
{
    ui->budgets->setText(controller->getBudgetData()->getBudgetString());
}

/* Function used to update the metric in display
 * @modifies this->ui->metrics
 * @effects this->ui->metrics now contains the recent metric information
 */
void DashBoard::updateMetrics()
{
    // Ask controller for metric data (basically a preformated string)

/*    delete ui->metrics->takeWidget();

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


    ui->metrics->setWidget(mWidget);*/
}


// Slots -- aka signals

// Function to switch to the input view when "Predict" button is pressed
void DashBoard::getInputView()
{
  this->controller->switchToInputWages();
  return;
}

// Function to start the Budget Modal when the "Budget" button is pressed
void DashBoard::getBudgetView()
{
  std::cout<<"HelloWorld"<<std::endl;
  controller->switchToBudgetPage();
  return;
}

// Function to show the metric as a pie chart
void DashBoard::getMetricsPieView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to show the metric as a vertical bar graph
void DashBoard::getMetricsBarView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to show the metric as a line graph
void DashBoard::getMetricsLineView()
{
  std::cout<<"HelloWorld"<<std::endl;
  return;
}

// Function to show the loaded budget as a pie chart
void DashBoard::getBudgetPieView()
{

}

// Function to show the loaded budget as a bar graph
void DashBoard::getBudgetBarView()
{

}
