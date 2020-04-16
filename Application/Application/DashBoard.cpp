#include "DashBoard.h"
#include "ui_DashBoard.h"

DashBoard::DashBoard(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::DashBoard)
{
    ui->setupUi(this);
    this->controller = controller;

    metrics = nullptr;
    //metricsLine = nullptr;

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
    delete metrics;
}

// Function used to update welcome message
/* @modifies ui->Message
 * @effecst ui->Message = "Welcome " userId
 */
void DashBoard::updateMessage(QString userId)
{
    ui->Message->setText(QString("Welcome ") + userId);
}

/* Function used to update the budget in display
 * @modifies this->ui->Budget
 * @effects this->ui->Budget now contains the budget that was recently loaded in
 */
void DashBoard::updateBudget(QString budgetId)
{
    ui->budgets->setText(controller->getBudgetData(budgetId)->getBudgetString());
}

/* Function used to update the metric in display
 * @modifies this->ui->metrics, this->metrics
 * @effects this->ui->metrics now contains the recent metric information
 *          this->metrics contains the Controller's metrics
 */
void DashBoard::updateMetrics()
{
    // Ask controller for metric data (basically a preformated string)
    const std::vector<double> * data = controller->getMetricsData();
    if (!data->empty())
    {
        std::vector<double>::const_iterator i = data->begin();
        if (metrics == nullptr)
        {
            metrics = new ChartMap;
            metrics->insert(std::pair<std::string, double>("Wages", *(i++)));
            metrics->insert(std::pair<std::string, double>("Investment", *(i++)));
            metrics->insert(std::pair<std::string, double>("Assets", *(i++)));
        }
        else
        {
            (*metrics)["Wages"] = *(i++);
            (*metrics)["Investment"] = *(i++);
            (*metrics)["Assets"] = *(i++);
        };

        i = data->begin();
        double sum = 0;
        while (i != (data->end()-1)) {sum += *(i++);}
        std::string years = QString::number((*(data->end() - 1)), 'f', 2).toStdString();

        ui->metrics->setText(QString::fromStdString("Your income will grow/shrink by " + QString::number(sum, 'f', 2).toStdString() + " dollars in " + years
                                                    + "\n\nYour wages will grow/shrink by " + QString::number((*metrics)["Wages"], 'f', 2).toStdString() + " dollars in " + years
                                                    + "\nYour investments will grow/shrink by " + QString::number((*metrics)["Investment"], 'f', 2).toStdString() + " dollars in " + years
                                                    + "\nYour assets will grow/shrink by " + QString::number((*metrics)["Assets"], 'f', 2).toStdString() + " dollars in " + years));
    }
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
  controller->switchToBudgetPage();
  return;
}

void DashBoard::getAccountManageView() {
    controller->switchToAccountManage();
    return;
}

// Function to show the metric as a pie chart
// @requires this->metric to be populated with 0's or actual metric data
void DashBoard::getMetricsPieView()
{
  controller->getPieChart(metrics)->show();
  return;
}

// Function to show the metric as a vertical bar graph
// @requires this->metric to be populated with 0's or actual metric data
void DashBoard::getMetricsBarView()
{
    controller->getBarGraph(metrics)->show();
    return;
}

// Function to show the metric as a line graph
// @requires this->metric to be populated with 0's or actual metric data
void DashBoard::getMetricsLineView()
{
    //this->updateMetrics();
    //controller->getLineGraph(metricsLine)->show();
    return;
}

// Function to show the loaded budget as a pie chart
void DashBoard::getBudgetPieView()
{
    const ChartMap * data = controller->getBudgetData(QString(""))->getBudgetChartMap();
    controller->getPieChart(data)->show();
}

// Function to show the loaded budget as a bar graph
void DashBoard::getBudgetBarView()
{
    const ChartMap * data = controller->getBudgetData(QString(""))->getBudgetChartMap();
    controller->getBarGraph(data)->show();
}
