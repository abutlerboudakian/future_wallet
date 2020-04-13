#include "Menu.h"

Menu::Menu(QWidget *parent, Controller * controller) : QMenuBar(parent)
{
    this->controller = controller;
    MainTab = new QMenu(this);
    QAction * AccountPage = new QAction(MainTab);
    AccountPage->setText("User Account Page");
    QAction * LogOut = new QAction(MainTab);
    LogOut->setText("Log Out");

    MainTab->addAction(AccountPage);
    MainTab->addAction(LogOut);
    this->addMenu(MainTab);

    connect(LogOut, SIGNAL(triggered()), this, SLOT(logout()));
    connect(AccountPage, SIGNAL(triggered()), this, SLOT(getUserAccountView()));
}

Menu::~Menu()
{

}

/* Function sets the username for the menubar
 * @param userId is a QString of the username
 * @modifies the menubar
 * @effect the menubar displays the username now
 */
void Menu::setUserName(QString userId)
{
    MainTab->setTitle(userId);
}



// Function gets the user account info page
void Menu::getUserAccountView()
{

}

/* Function Logs the user out */
void Menu::logout()
{
    this->controller->logout();
}
