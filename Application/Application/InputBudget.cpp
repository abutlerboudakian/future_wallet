#include "InputBudget.h"
#include "ui_InputBudget.h"

InputBudget::InputBudget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputBudget)
{
    ui->setupUi(this);
}

InputBudget::~InputBudget()
{
    delete ui;
}

/* Function used to get all the category budget data
 *
 */
void InputBudget::getCategoryData() {
    std::cout<<"Start"<<std::endl;
    std::unordered_map<std::string, int> catData;

    std::cout<<ui->Categories->count()<<std::endl;

    for (int i = 0; i < ui->Categories->count(); i++)
    {
        QList<QLineEdit*> catName = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QLineEdit*>(QRegularExpression(QString("/Category\\d/g")));
        QList<QLineEdit*> catVal = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QLineEdit*>(QRegularExpression(QString("/Slider\\d/g")));

        std::cout<<catName[0]->text().toStdString()<<catName[0]->text().toStdString();

        catData.insert(std::pair<std::string, int>(catName[0]->text().toStdString(), catVal[0]->text().toDouble()));
    }

    for (std::unordered_map<std::string, int>::iterator i = catData.begin(); i != catData.end(); i++)
    {
        std::cout<<i->first<<" "<<i->second<<std::endl;
    }

    std::cout<<"Done"<<std::endl;
}

// Function to save budget and then save to dashboard with updated budget
void InputBudget::Create() {
    // Add code to tell controller to update it's AssetModel
    this->controller->switchToDashBoard();
}

// Function to switch view back to the dashboard and save current
// progress in controller
void InputBudget::Exit() {
    // Add code to tell controller to update it's AssetModel
    this->controller->switchToDashBoard();
}

// Function modifies the ui to add a new category field
/* @modifies this->ui
 * @effect this->ui->Categories has a new category field
 *
 */
void InputBudget::addCategory() {
   return;
}

// Function modifies the ui to remove a category field
/* @modifies this->ui
 * @effect this->ui->Stocks has one less category field. Could possibly fully remove all categories.
 */
void InputBudget::removeCategory() {
    if (this->ui->Categories->count() > 1) {
        QPushButton * removeButton = (QPushButton*)sender();
        QWidget * target = removeButton->parentWidget();
        this->ui->Categories->removeWidget(target);
        delete target;
    }
}

// Function to modify label that has respective slider
/* @modifies this->ui
 */
void InputBudget::updateLabel(QSlider * slider, QLabel *label) {
    return;
}
