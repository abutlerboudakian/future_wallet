#include "LaunchMenu.h"
#include "ui_LaunchMenu.h"

LaunchMenu::LaunchMenu(QWidget *parent, Controller * controller) :
    QWidget(parent),
    ui(new Ui::LaunchMenu)
{
    ui->setupUi(this);

    this->controller = controller;
    ui->PassField->setEchoMode(QLineEdit::Password);

    // Bind slots
    connect(ui->Login, SIGNAL(released()), this, SLOT(runLogin()));
    connect(ui->CreateAccount, SIGNAL(released()), this, SLOT(runCreateAccount()));
}

// Delegates destruction of QObjects to base QWidget destructor
LaunchMenu::~LaunchMenu()
{
    delete ui;
}

//-------------------------------------
// Slots                              |
//-------------------------------------

// Function to call the login function in the controller
void LaunchMenu::runLogin()
{
    controller->login(ui->UserField->text(), ui->PassField->text());
    return;
}

// Function to switch views to Registration
void LaunchMenu::runCreateAccount()
{
    controller->switchToRegisterPage();
    return;
}
