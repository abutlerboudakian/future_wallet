#include "LaunchMenu.h"
#include <iostream>

LaunchMenu::LaunchMenu(QMainWindow * parent) : QWidget(parent)
{
  // Create the Label
  TitleBar = new QLabel(QString("WELCOME TO FUTURE WALLET"));
  TitleBar->setStyleSheet(QString("font-size:20pt;\ncolor:blue"));

  // Create the Login Form
  Form = new QFormLayout;
  Form->addRow(tr("Usernane:"), UserLineEdit);
  Form->addRow(tr("Email:"), EmailLineEdit);

  // Create submission buttons
  Login = new QPushButton("Login");
  CreateAccount = new QPushButton("Create an account");

  Buttons = new QGridLayout;
  Buttons->addWidget(Login, 0, 1, Qt::AlignRight);
  Buttons->addWidget(CreateAccount, 0, 0, Qt::AlignRight);

  MainLayout = new QGridLayout;
  MainLayout->addWidget(TitleBar, 0, 0);
  MainLayout->addWidget(Form, 1, 0);
  MainLayout->addWidget(Buttons, 2, 0);

  this->setLayout(MainLayout);

  // Bind slots
  connect(Login, SIGNAL(released()), this, SLOT(runLogin()));
  connect(CreateAccount, SIGNAL(released()), this, SLOT(runCreateAccount()));
}

LaunchMenu::~LaunchMenu()
{
  delete MainLayout;
  delete Form;
}

// Slots
void runLogin()
{
  std::cout<<"Hello World";
  return;
}

void runCreateAccount()
{
  std::cout<<"Hello World";
  return;
}