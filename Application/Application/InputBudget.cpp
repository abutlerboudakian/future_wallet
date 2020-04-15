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
