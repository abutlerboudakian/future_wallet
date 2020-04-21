#include "Registration.h"
#include "ui_Registration.h"

Registration::Registration(QWidget *parent, Controller *controller) :
  QWidget(parent),
  ui(new Ui::Registration)
{
  ui->setupUi(this);

  this->controller = controller;
  ui->passInput->setEchoMode(QLineEdit::Password);
  ui->confirmPassInput->setEchoMode(QLineEdit::Password);

  // Bind slots
  connect(ui->btnRegister, SIGNAL(released()), this, SLOT(registerUser()));
  connect(ui->btnCancel, SIGNAL(released()), this, SLOT(cancelRegistration()));
}

Registration::~Registration()
{
  delete ui;
}

//-------------------------------------
// Slots                              |
//-------------------------------------

/* Function used to ask the controller to try registering a new user given the user info
 */
void Registration::registerUser()
{
  if (ui->passInput->text() != ui->confirmPassInput->text())
  {
      QMessageBox * errModal = new QMessageBox(QMessageBox::Critical, "Error", "Passwords don't match. Please try again.");
      errModal->setAttribute(Qt::WA_DeleteOnClose, true); // Deconstruct on closing
      errModal->show();
  }
  else
  {
      controller->Register(ui->emailInput->text(), ui->passInput->text());
  }
}

/* Function to clear the fields of the registration page
 * @modifies ui->emailInput, ui->passInput, ui->confirmPassInput
 * @effect ui->emailInput = ui->passInput = ui->confirmPassInput = ""
 */
void Registration::clear()
{
    ui->emailInput->setText("");
    ui->passInput->setText("");
    ui->confirmPassInput->setText("");
}

/* FUnction used to switch back to the login view and clear the registration fields
 */
void Registration::cancelRegistration()
{
  this->clear();
  controller->switchToLogin();
}
