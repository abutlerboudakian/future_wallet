#include "LaunchMenu.h"

LaunchMenu::LaunchMenu(QMainWindow * parent, QStackedWidget * content) : QWidget(parent)
{
  this->content = content;

  // Create the Label
  TitleBar = new QLabel(QString("WELCOME TO FUTURE WALLET"));
  TitleBar->setStyleSheet(QString("font-size:20pt;\ncolor:blue"));

  // Create the Login Form
  Form = new QFormLayout;
  UserLineEdit = new QLineEdit;
  PassLineEdit = new QLineEdit;
  Form->addRow(tr("Usernane:"), UserLineEdit);
  Form->addRow(tr("Password:"), PassLineEdit);

  // Create submission buttons
  Login = new QPushButton("Login");
  CreateAccount = new QPushButton("Create an account");

  Buttons = new QGridLayout;
  Buttons->addWidget(Login, 0, 1, Qt::AlignRight);
  Buttons->addWidget(CreateAccount, 0, 0, Qt::AlignRight);

  MainLayout = new QGridLayout;
  MainLayout->addWidget(TitleBar, 0, 0, Qt::AlignCenter);
  MainLayout->addLayout(Form, 1, 0, Qt::AlignCenter);
  MainLayout->addLayout(Buttons, 2, 0, Qt::AlignRight);

  this->setLayout(MainLayout);

  // Bind slots
  connect(Login, SIGNAL(released()), this, SLOT(runLogin()));
  connect(CreateAccount, SIGNAL(released()), this, SLOT(runCreateAccount()));
}

LaunchMenu::~LaunchMenu()
{
  delete MainLayout;
  delete Form;
  delete Login;
  delete CreateAccount;
  delete Buttons;
  delete UserLineEdit;
  delete PassLineEdit;
}

// Slots
void LaunchMenu::runLogin()
{
  std::cout<<"Hello World";
  this->content->setCurrentIndex(1);
  return;
}

void LaunchMenu::runCreateAccount()
{
  std::cout<<"Hello World";
  return;
}
