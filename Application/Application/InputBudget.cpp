#include "InputBudget.h"
#include "ui_InputBudget.h"

InputBudget::InputBudget(QWidget *parent,  Controller * controller) :
    QWidget(parent),
    ui(new Ui::InputBudget)
{
    // signalMapper = new QSignalMapper(this);
    ui->setupUi(this);
    this->controller = controller;

    connect(ui->Cancel, SIGNAL(released()), this, SLOT(Exit()));
    connect(ui->Create, SIGNAL(released()), this, SLOT(Create()));
    connect(ui->AddCat, SIGNAL(released()), this, SLOT(addCategory()));
    connect(ui->RemoveCat, SIGNAL(released()), this, SLOT(removeCategory()));

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
}

InputBudget::~InputBudget()
{
    delete ui;
}

/* Function used to get all the category budget data
 *
 */
void InputBudget::getCategoryData() {
    std::unordered_map<std::string, double> catData;
    // Get name of budget
    QLabel *budgetLabel = this->findChild<QLabel*>(QString::fromStdString("BudgetName"));
    std::string budgetName = budgetLabel->text().toStdString();

    for (int i = 0; i < ui->Categories->count(); i++)
    {
        // Get category name value for layout
        std::string catLabel = "Category" + std::to_string(i);
        QLineEdit *catName = this->findChild<QLineEdit*>(QString::fromStdString(catLabel));

        // Get category int value for layout
        std::string sliderLabel = "Slider" + std::to_string(i);
        QSlider *sliderVal = this->findChild<QSlider*>(QString::fromStdString(catLabel));

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
    std::string valueName = "Value" + std::to_string(counter - 1);
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
}

// Function to modify label whenever its respective slider is modified by user
/* @modifies this->ui->label to value of slider
 */
void InputBudget::updateLabel(QSlider *slider, QLabel *label) {
    label->setText(QString::number(slider->value()));
}
