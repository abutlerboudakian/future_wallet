#include "InputBudget.h"
#include "ui_InputBudget.h"

InputBudget::InputBudget(QWidget *parent,  Controller * controller) :
    QWidget(parent),
    ui(new Ui::InputBudget)
{
    // signalMapper = new QSignalMapper(this);
    ui->setupUi(this);
    this->controller = controller;
    this->budget = new BudgetData;//nullptr;

    connect(ui->Cancel, SIGNAL(released()), this, SLOT(Exit()));
    connect(ui->Create, SIGNAL(released()), this, SLOT(Create()));
    connect(ui->AddCat, SIGNAL(released()), this, SLOT(addCategory()));
    connect(ui->RemoveCat, SIGNAL(released()), this, SLOT(removeCategory()));
    connect(ui->nameBudget, SIGNAL(textEdited(QString)), this, SLOT(updateBudgetName()));

    // For all possible sliders, listen for a possible value change
    // TODO: Breaks the web-app
    /**
        QList<QSlider*> slider = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QSlider*>(QRegularExpression(QString("/Slider\\d/g")));
        QList<QLabel*> label = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QLabel*>(QRegularExpression(QString("/Value\\d/g")));

        connect(slider[0], SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
        signalMapper->setMapping(slider[0], label[0]);
        connect(signalMapper, SIGNAL(mapped(QWidget*)),this, SLOT(updateLabel(slider[0], label[0])));
    **/

    // Connect the slider to the qlabel
    QString sliderName = QString("Slider");
    QString sliderLabel = QString("ValueSlider");
    for (int i = 0; i < ui->Categories->count(); i++)
    {
        connect(this->findChild<QSlider*>(sliderName + QString::number(i)), SIGNAL(valueChanged(int)), this, SLOT(updateLabel(int)));
    }
}

InputBudget::~InputBudget()
{
    delete ui;
    delete this->budget;
}

/* Function used to reset this->budget and the view
 * @modifies this->budget and other view elements to the default
 * @effect this->budget = new BugetData()
 *         the create budget page is reset to the default
 */
void InputBudget::resetBudget()
{
    // Reset this->budget
    if (this->budget != nullptr)
    {
        delete this->budget;
    }
    this->budget = new BudgetData;
    this->budget->addCategory("Category 0", 0);
    this->budget->addCategory("Category 1", 0);
    this->budget->addCategory("Category 2", 0);
    this->budget->addCategory("Category 3", 0);

    // Reset the view
    // DO THIS LATER
}

/* Function used to get all the category budget data
 *
 */
void InputBudget::getCategoryData() {
    std::unordered_map<std::string, double> catData;
    // Get name of budget
    QLineEdit *budgetLabel = this->findChild<QLineEdit*>(QString::fromStdString("nameBudget"));
    std::string budgetName = budgetLabel->text().toStdString();

    for (int i = 0; i < ui->Categories->count(); i++)
    {
        // Get category name value for layout
        std::string catLabel = "Category" + std::to_string(i);
        QLineEdit *catName = this->findChild<QLineEdit*>(QString::fromStdString(catLabel));

        // Get category int value for layout
        std::string sliderLabel = "Slider" + std::to_string(i);
        QSlider *sliderVal = this->findChild<QSlider*>(QString::fromStdString(sliderLabel));

        // Cout values to check
        std::cout << catName->text().toStdString() << sliderVal->value();

        double tickValue = ( sliderVal->value() )/ 100;
        catData.insert(std::pair<std::string, int>(catName->text().toStdString(), tickValue));
    }

    for (std::unordered_map<std::string, double>::iterator i = catData.begin(); i != catData.end(); i++)
    {
        std::cout<<i->first<<" "<<i->second<<std::endl;
    }
}

// Function to save budget and then switch to dashboard with updated budget
void InputBudget::Create() {
    // Add code to save budget and categories
    controller->addBudget(this->budget);
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
    counter++;
    // Horizontal layout that will contain new category
    QHBoxLayout *horizonalLayout = new QHBoxLayout;
    std::string layoutName = "CategoryLayout" + std::to_string(counter - 1);
    horizonalLayout->setObjectName(QString::fromStdString(layoutName));

    // New category line edit
    QLineEdit *newCategory = new QLineEdit;
    std::string catName = "Category" + std::to_string(counter - 1);
    newCategory->setObjectName(QString::fromStdString(catName));

    // New slider
    QSlider *slider = new QSlider;
    std::string sliderName = "Slider" + std::to_string(counter - 1);
    slider->setObjectName(QString::fromStdString(sliderName));
    slider->setMaximum(100);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);

    // New value of slider
    QLabel * valueOfSlider = new QLabel;
    std::string valueName = "ValueSlider" + std::to_string(counter - 1);
    valueOfSlider->setObjectName(QString::fromStdString(valueName));
    valueOfSlider->setText(QCoreApplication::translate("InputBudget", "0.00", nullptr));

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

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateLabel(int)));
    updateLimits();
}

// Function modifies the ui to remove a category field
/* @modifies this->ui
 * @effect this->ui->Stocks has one less category field, unless only 1 budget category is left. Error message pops up if user attempts to delete
 * all budgets
 */
void InputBudget::removeCategory() {
    // There should always be at least 1 budget category
    if (this->ui->Categories->count() > 1) {
        std::string lastLayoutStr = "CategoryLayout" + std::to_string(counter - 1);

        // Remove the category from the internal budget representation
        QString sliderNumber = QString::number(counter - 1);
        this->budget->removeCategory(this->findChild<QLineEdit*>(QString("Category") + sliderNumber)->text());

        QHBoxLayout* lastLayout = ui->Categories->findChild<QHBoxLayout*>(QString::fromStdString(lastLayoutStr));
        while(!lastLayout->isEmpty()) {
            QWidget *target = lastLayout->takeAt(0)->widget();
            delete target;
        }
        delete lastLayout;
        counter--;
    } else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","At least one category required for budget.");
        messageBox.setFixedSize(500,200);
        return;
    }
    updateLimits();
}

// Function to modify label whenever its respective slider is modified by user
/* @modifies this->ui->label to value of slider
 */
void InputBudget::updateLabel(int val) {
    // If increasing, check if increase to value is allowed. decrease all slider maxes to min(val + totalRemaining, 100)
    // If decreasing, expand all slider maxes to min(val + totalRemaining, 100)

    QSlider * caller = (QSlider*)sender();
    QString sliderNumber = caller->objectName().split("Slider")[1];
//qDebug()<<val<<" "<<val/100.0 << " "<<caller->maximum();
    if (val > caller->maximum())
    {
        caller->setSliderPosition(caller->maximum()); // So it doesn't move past it
        return;
    }

    // Check if increase to value is allowed
    QString catName = this->findChild<QLineEdit*>(QString("Category") + sliderNumber)->text();
    double originalValue = this->budget->getCategoryValue(catName);
    if (originalValue != -1) {this->budget->removeCategory(catName);} // Remove if category exists
    if (this->budget->addCategory(catName, double(val)/100))
    {   // Can update value display
        this->findChild<QLabel *>(QString("ValueSlider") + sliderNumber)->setText(QString::number(val) + QString("%"));
    }
    else
    {   // Can't change category value, so don't update display and revert
        this->budget->addCategory(catName, originalValue);
    }

    // Update limits for all sliders
    updateLimits();
}

/* Function updates slider limits to match 100% budget limitation
 * @modifies this->sliders
 * @effect this->sliders max limit are updated.
 */
void InputBudget::updateLimits()
{
    double remaining = this->budget->getRemaining(), newMax;
    QString name = QString("Slider");
    QSlider * temp;
    for (int i = 0; i < ui->Categories->count(); i++)
    {
        temp = this->findChild<QSlider *>(name + QString::number(i));
        newMax = temp->value() + remaining;
        //qDebug() << name << i<< " "<<(temp->value()/100.0)<<" "<<remaining<<" "<<newMax << " " << (newMax < 100.0 ? newMax : 100.0);
        newMax = (newMax < 100.0 ? newMax : 100.0);
        temp->setMaximum(newMax);
    }
    //qDebug() << this->budget->getRemaining();
}


/* Function used to update the budgetname for the budgetdata class
 * @modifies this->budget
 * @effect this->budget->getName() = ui->nameBudget
 */
void InputBudget::updateBudgetName()
{
    this->budget->setName(((QLineEdit*)sender())->text());
}
