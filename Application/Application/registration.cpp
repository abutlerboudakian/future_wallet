#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent, Controller *controller) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);

    this->controller = controller;
    ui->passInput->setEchoMode(QLineEdit::Password);
    ui->confirmPassInput->setEchoMode(QLineEdit::Password);

    connect(ui->btnRegister, SIGNAL(released()), this, SLOT(registerUser()));
    connect(ui->btnCancel, SIGNAL(released()), this, SLOT(cancelRegistration()));
}

Registration::~Registration()
{
    delete ui;
}


void Registration::registerUser()
{
    if (ui->passInput->text() != ui->confirmPassInput->text())
    {
        QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Passwords don't match. Please try again.");
        errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
    }
    else
        controller->Register(ui->emailInput->text(), ui->passInput->text());
}

void Registration::cancelRegistration()
{
    ui->emailInput->setText("");
    ui->passInput->setText("");
    ui->confirmPassInput->setText("");
    controller->switchToLogin();
}
