#include "InputBudget.h"
#include "ui_InputBudget.h"

InputBudget::InputBudget(QWidget *parent,  Controller * controller) :
    QWidget(parent),
    ui(new Ui::InputBudget)
{
    // signalMapper = new QSignalMapper(this);
    ui->setupUi(this);
    this->controller = controller;
    this->budget = nullptr;
    this->resetBudget();

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
    //connect(ui->Slider0, SLOT(valueChanged(int)), this, SLOT(updateLabel(int)));

    /*QString sliderName = QString("Slider");
    for (int i = 0; i < ui->Categories->count(); i++)
    {
        connect(this->findChild<QSlider*>(sliderName + QString::number(i)), SIGNAL(valueChanged(int)), this, SLOT(updateLabel(int)));
    }*/
    connect(this->findChild<QSlider*>(QString("Slider0")), SIGNAL(valueChanged(int)), this, SLOT(updateLabel(int)));
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
    this->budget->addCategory("Category0", 0);

    // Reset the view
    ui->nameBudget->setText(QString("Budget Name"));
    ui->Category0->setText(QString("Category 0"));
    ui->Slider0->setValue(0);

    // Remove Excess Categories
    QString CatContainer = QString("CategoryLayout");
    QWidget * Category;
    qDebug() << ui->Categories->count();
    int numCats = ui->Categories->count(); // since this is prone to change via removeWidget
    for (int i = 1; i < numCats; i++)
    {
        Category = this->findChild<QWidget*>(CatContainer + QString::number(i));
        ui->Categories->removeWidget(Category);
        delete Category;
    }
    counter = 1;
}

/* Function used to get all the category budget data
 *
 */
/*void InputBudget::getCategoryData() {
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
}*/

// Function to save budget and then switch to dashboard with updated budget
void InputBudget::Create() {
    // Add code to save budget and categories
    QString UIName = QString("Category"), CurrentUIName;
    QString catName;
    BudgetData * Message = new BudgetData();
    for (int i = 0; i < ui->Categories->count(); i++)
    {
        CurrentUIName = UIName + QString::number(i);
        catName = this->findChild<QLineEdit*>(CurrentUIName)->text();
        if (catName == QString("") || !Message->addCategory(catName, this->budget->getCategoryValue(CurrentUIName)))
        {
            QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", QString("Invalid Budget Logic. Please Try again."));
            errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
            errModal->show();
            delete Message;
            return;
        }
    }
    delete this->budget;
    this->budget = Message;
    this->budget->setName(ui->nameBudget->text());
    if (controller->addBudget(this->budget))
    {
        this->controller->switchToDashBoard();
    }
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
    QWidget * Category = new QWidget();
    std::string layoutName = "CategoryLayout" + std::to_string(counter - 1);
    Category->setObjectName(QString::fromStdString(layoutName));

    QHBoxLayout *horizonalLayout = new QHBoxLayout(Category); // Layout of Category

    // New category line edit
    QLineEdit *newCategory = new QLineEdit(Category);
    std::string catName = "Category" + std::to_string(counter - 1);
    newCategory->setObjectName(QString::fromStdString(catName));

    // New slider
    QSlider *slider = new QSlider(Category);
    std::string sliderName = "Slider" + std::to_string(counter - 1);
    slider->setObjectName(QString::fromStdString(sliderName));
    slider->setMaximum(100);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);

    // New value of slider
    QLabel * valueOfSlider = new QLabel(Category);
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
    Category->setLayout(horizonalLayout);
    ui->Categories->addWidget(Category);

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
        QString CatName = QString("Category") + QString::number(counter - 1);
        this->budget->removeCategory(CatName);

        QWidget * lastCategory = this->findChild<QWidget*>(QString::fromStdString(lastLayoutStr));
        ui->Categories->removeWidget(lastCategory);
        delete lastCategory;
        counter--;
        updateLimits();
    } else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","At least one category required for budget.");
        messageBox.setFixedSize(500,200);
        return;
    }
}

// Function to modify label whenever its respective slider is modified by user
/* @modifies this->ui->label to value of slider
 */
void InputBudget::updateLabel(int val) {
    // If increasing, check if increase to value is allowed. decrease all slider maxes to min(val + totalRemaining, 100)
    // If decreasing, expand all slider maxes to min(val + totalRemaining, 100)

    QSlider * caller = (QSlider*)sender();
    QString sliderNumber = caller->objectName().split("Slider")[1];
qDebug()<<val<<" "<<val/100.0 << " "<<caller->maximum();
    if (val > caller->maximum())
    {
        caller->setSliderPosition(caller->maximum()); // So it doesn't move past it
        return;
    }

    // Check if increase to value is allowed
    QString catName = QString("Category") + sliderNumber;
    double originalValue = this->budget->getCategoryValue(catName);
    if (originalValue != -1) {this->budget->removeCategory(catName);} // Remove if category exists
    if (this->budget->addCategory(catName, double(val)/100))
    {   // Can update value display
        this->findChild<QLabel *>(QString("ValueSlider") + sliderNumber)->setText(QString::number(val) + QString("% | $") +
                                                                                  QString::number(this->budget->getDollar() * (double(val)/100), 'f', 2));
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
        qDebug() << name << i<< " "<<(temp->value()/100.0)<<" "<<remaining<<" "<<newMax << " " << (newMax < 100.0 ? newMax : 100.0);
        newMax = (newMax < 100.0 ? newMax : 100.0);
        temp->setMaximum(newMax);
    }
    qDebug() << this->budget->getRemaining();
}


/* Function used to update the budgetname for the budgetdata class
 * @modifies this->budget
 * @effect this->budget->getName() = ui->nameBudget
 */
void InputBudget::updateBudgetName()
{
    this->budget->setName(((QLineEdit*)sender())->text());
}
