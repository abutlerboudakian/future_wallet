#include "LaunchMenu.h"

LaunchMenu::LaunchMenu(QMainWindow * parent, Controller * controller) : QWidget(parent)
{
  this->controller = controller;
  QLabel* TitleBar;
  QFormLayout* Form;
  QGridLayout* Buttons;
  QPushButton* CreateAccount;
  QPushButton* Login;
  QLineEdit* UserLineEdit;
  QLineEdit* PassLineEdit;

  // Create the Label
  TitleBar = new QLabel(QString("WELCOME TO FUTURE WALLET"), this);
  TitleBar->setStyleSheet(QString("font-size:20pt;\ncolor:blue"));

  // Create the Login Form
  Form = new QFormLayout(this);
  UserLineEdit = new QLineEdit(this);
  PassLineEdit = new QLineEdit(this);
  PassLineEdit->setEchoMode(QLineEdit::Password);
  Form->addRow(tr("Usernane:"), UserLineEdit);
  Form->addRow(tr("Password:"), PassLineEdit);

  // Create submission buttons
  Login = new QPushButton("Login", this);
  CreateAccount = new QPushButton("Create an account", this);

  Buttons = new QGridLayout(this);
  Buttons->addWidget(Login, 0, 1, Qt::AlignRight);
  Buttons->addWidget(CreateAccount, 0, 0, Qt::AlignRight);

  MainLayout = new QGridLayout(this);
  MainLayout->addWidget(TitleBar, 0, 0, Qt::AlignCenter);
  MainLayout->addLayout(Form, 1, 0, Qt::AlignCenter);
  MainLayout->addLayout(Buttons, 2, 0, Qt::AlignRight);

  this->setLayout(MainLayout);

  // Bind slots
  connect(Login, SIGNAL(released()), this, SLOT(runLogin()));
  connect(CreateAccount, SIGNAL(released()), this, SLOT(runCreateAccount()));
}

// Delegates destruction of QObjects to base QWidget destructor
LaunchMenu::~LaunchMenu()
{
  delete MainLayout;
}

// Slots
void LaunchMenu::runLogin()
{
  std::cout<<"Hello World";
  controller->switchToDashBoard();
  return;
}

void LaunchMenu::runCreateAccount()
{
  std::cout<<"Hello World";
  return;
}
