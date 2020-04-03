#include "BudgetPage.h"
#include "ui_BudgetPage.h"

BudgetPage::BudgetPage(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::BudgetPage),
    BaseView()
{
    ui->setupUi(this);

    this->controller = controller;

    // Bind slots
    connect(ui->LoadBudget, SIGNAL(released()), this, SLOT(getLoadBudgetView()));
    connect(ui->CreateBudget, SIGNAL(released()), this, SLOT(getCreateBudgetView()));
}

BudgetPage::~BudgetPage()
{
    delete ui;
}

/* Function to switch views to the chart page, telling
 * the controller what budget to laod
 */
void BudgetPage::getLoadBudgetView()
{
    // Get the budget id to load
    // controller->setSelectedBudget

    controller->switchToChartSelection();
}

void BudgetPage::getCreateBudgetView()
{

}
