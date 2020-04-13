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
    std::unordered_map<std::string, double> catData;

    std::cout<<ui->Categories->count()<<std::endl;

    for (int i = 0; i < ui->Categories->count(); i++)
    {
        QList<QLineEdit*> catName = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QLineEdit*>(QRegularExpression(QString("/Category\\d/g")));
        QList<QSlider*> catVal = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QSlider*>(QRegularExpression(QString("/Slider\\d/g")));

        std::cout<<catName[0]->text().toStdString()<<catName[0]->text().toStdString();

        double tickValue = ( catVal[0]->value() )/ 100;
        catData.insert(std::pair<std::string, int>(catName[0]->text().toStdString(), tickValue));
    }

    for (std::unordered_map<std::string, double>::iterator i = catData.begin(); i != catData.end(); i++)
    {
        std::cout<<i->first<<" "<<i->second<<std::endl;
    }

    std::cout<<"Done"<<std::endl;
}

// Function to save budget and then switch to dashboard with updated budget
void InputBudget::Create() {
    // Add code to save budget and categories
    InputBudget::getCategoryData();
    this->controller->switchToDashBoard();
}

// Function to switch view back to the dashboard and abandon budget
void InputBudget::Exit() {
    this->controller->switchToDashBoard();
}

// Function modifies the ui to add a new category field
/* @modifies this->ui
 * @effect this->ui->Categories has a new category field
 *
 */
void InputBudget::addCategory() {
    // Horizontal layout that will contain new category
    QHBoxLayout *horizonalLayout = new QHBoxLayout();
    std::string layoutName = "horizontalLayout_" + std::to_string(counter);
    horizonalLayout->setObjectName(QString::fromUtf8(layoutName));

    // New category line edit
    QLineEdit *newCategory = new QLineEdit;
    std::string catName = "Category" + std::to_string(counter);
    newCategory->setObjectName(QString::fromUtf8(catName));

    // New slider
    QSlider *slider = new QSlider;
    std::string sliderName = "Slider" + std::to_string(counter);
    slider->setObjectName(QString::fromUtf8(sliderName));
    slider->setMaximum(100);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);

    // New value of slider
    QLabel *valueOfSlider = new QLabel;
    std::string valueName = "Value" + std::to_string(counter - 1);
    valueOfSlider->setObjectName(QString::fromUtf8(valueName));

    // Horizontal spacer/size policy of layout
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(newCategory->sizePolicy().hasHeightForWidth());

    newCategory->setSizePolicy(sizePolicy);
    newCategory->setMinimumSize(QSize(0, 0));

    // Add all newly created widgets and items to layout
    horizonalLayout->addWidget(newCategory);
    horizonalLayout->addItem(horizontalSpacer);
    horizonalLayout->addWidget(slider);
    horizonalLayout->addWidget(valueOfSlider);

    // Add layout to categories
    ui->Categories->addLayout(horizonalLayout);
}

// Function modifies the ui to remove a category field
/* @modifies this->ui
 * @effect this->ui->Stocks has one less category field, unless only 1 budget category is left. Preferably an error message pops up.
 */
void InputBudget::removeCategory() {
    // There should always be at least "counter" amount of budgets
    if (this->ui->Categories->count() > counter) {
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
