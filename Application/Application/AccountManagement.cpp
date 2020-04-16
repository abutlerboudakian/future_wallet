#include "AccountManagement.h"
#include "ui_AccountManagement.h"

AccountManagement::AccountManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountManagement)
{
    ui->setupUi(this);
}

AccountManagement::~AccountManagement()
{
    delete ui;
}
