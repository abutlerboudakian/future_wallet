#include "BudgetPage.h"
#include "ui_BudgetPage.h"

BudgetPage::BudgetPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BudgetPage)
{
    ui->setupUi(this);

    ui->Default->setSelectionMode(QListWidget::SingleSelection);
    ui->Custom->setSelectionMode(QListWidget::SingleSelection);
}

BudgetPage::~BudgetPage()
{
    delete ui;
}

/* Function to set the controller and event triggers for the view
 * @modifies this->controller
 * @effects this->controller = controller
 */
void BudgetPage::setController(Controller * controller)
{
    this->controller = controller;

    // Bind slots
    connect(ui->LoadBudget, SIGNAL(released()), this, SLOT(getLoadBudgetView()));
    connect(ui->CreateBudget, SIGNAL(released()), this, SLOT(getCreateBudgetView()));
    connect(ui->Default, SIGNAL(itemSelectionChanged()), this, SLOT(changeSelected()));
    connect(ui->Custom, SIGNAL(itemSelectionChanged()), this, SLOT(changeSelected()));
}

// Slots

/* Function to update DashBoardPage through
 * the controller with the budget to load
 */
void BudgetPage::getLoadBudgetView()
{
    // Get the budget id to load
    // Figure out if it is ui->Default or ui->Custom. Else, send error modal
    // ui->THING->currentItem()->text()
    // controller->setSelectedBudget
    QListWidget * selected;
    selected = (ui->Default->selectionModel()->hasSelection() ? ui->Default : ui->Custom);
    if (selected->selectionModel()->hasSelection())
    {
        controller->setSelectedBudget(selected->currentItem()->text());
    }
}

void BudgetPage::getCreateBudgetView()
{

}

/* Function ensures that only one item in either list can be selected
 * @modifies this->ui->Default and this->ui->Custom
 * @effect only on item in either this->ui->Default or this->ui->Custom, but not
 *         both is allowed to be selected.
 */
void BudgetPage::changeSelected()
{
  QListWidget * selected = (QListWidget*)QObject::sender(), *other;
  other = (selected == ui->Default ? ui->Custom : ui->Default);
  if (selected->selectionModel()->hasSelection() && other->selectionModel()->hasSelection())
  {
      other->selectionModel()->clearSelection(); // Clear the selection in the other listwidget
  }
}

/* Function to tell the controller that it is closing
 */
void BudgetPage::closeEvent(QCloseEvent * event)
{
    controller->closeBudgetPage(); // tells controller it is closing
    //destroy(true, true);
    event->accept();
}

/* Function used to update the list of user created budgets
 * @modifies this->Custom
 * @effect this->Custom is populate with a list of user created budget names
 */
void BudgetPage::updateUserList()
{
    ui->Custom->clear();
    ui->Custom->addItems(controller->getBudgetList());
}
