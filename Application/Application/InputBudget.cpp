#include "InputBudget.h"
#include "ui_InputBudget.h"

InputBudget::InputBudget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputBudget)
{
    signalMapper = new QSignalMapper(this);
    ui->setupUi(this);
    this->controller = controller;

    connect(ui->Cancel, SIGNAL(released()), this, SLOT(Exit()));
    connect(ui->Create, SIGNAL(released()), this, SLOT(Create()));
    connect(ui->AddCat, SIGNAL(released()), this, SLOT(addCategory()));
    connect(ui->RemoveCat, SIGNAL(released()), this, SLOT(removeCategory()));

    // For all possible sliders, listen for a possible value change
    for (int i = 0; i < ui->Categories->count(); i++)
    {
        QList<QSlider*> slider = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QSlider*>(QRegularExpression(QString("/Slider\\d/g")));
        QList<QLabel*> label = ((QWidget*) ui->Categories->itemAt(i))
                ->findChildren<QLabel*>(QRegularExpression(QString("/Value\\d/g")));

        connect(slider[0], SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
        signalMapper->setMapping(slider[0], label[0]);
        connect(signalMapper, SIGNAL(mapped(QWidget*)),this, SLOT(updateLabel(slider[0], label[0])));
    }
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
    QLabel *valueOfSlider = new QLabel;
    std::string valueName = "Value" + std::to_string(counter - 1);
    valueOfSlider->setObjectName(QString::fromStdString(valueName));

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
    counter++;
}

// Function modifies the ui to remove a category field
/* @modifies this->ui
 * @effect this->ui->Stocks has one less category field, unless only 1 budget category is left. Preferably an error message pops up.
 */
void InputBudget::removeCategory() {
    // There should always be at least "counter" amount of budgets
    if (this->ui->Categories->count() > counter) {
        std::string lastLayoutStr = "CategoryLayout" + std::to_string(counter - 1);
        QHBoxLayout* lastLayout = ui->Categories->findChild<QHBoxLayout*>(QString::fromStdString(lastLayoutStr));

        this->ui->Categories->removeItem(lastLayout);
        delete lastLayout;
    }
}

// Function to modify label whenever its respective slider is modified by user
/* @modifies this->ui->label to value of slider
 */
void InputBudget::updateLabel(QSlider *slider, QLabel *label) {
    label->setText(QString::number(slider->value()));
}
