#include "sideBar.h"

sideBar::sideBar(QWidget * parent) : QWidget(parent)
{
    // Sidebar code
    // QWidget * sidebar = new QWidget(this); // make parent 'this' so it is displayed on the MainWindow
    QPushButton * userAccount = new QPushButton("User Account"); // Add icons later using the other constructor
    QPushButton * logout = new QPushButton("LogOut");

    // Throw all elements onto the vboxlayout
    QVBoxLayout * verticalLayout = new QVBoxLayout(parent);
    verticalLayout->addWidget(userAccount);
    verticalLayout->addWidget(logout);
    /* QGridLayout * verticalLayout = new QGridLayout(parent);
    verticalLayout->addWidget() */

    // Create bg color
    const QColor darkSlatedBlue = QColor(72, 61, 139);
    QPalette sidebarBGColor = palette();
    sidebarBGColor.setColor(QPalette::Background, darkSlatedBlue);

    // Finish setting up the sidebar
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(sidebarBGColor);
    this->setMinimumWidth(100);
    this->setMaximumWidth(100);
    this->setMinimumHeight(500);
    this->setMaximumHeight(parent->height());
    // this->show();
}

void sideBar::updateHeight(QWidget *parent)
{
    this->setMinimumHeight(parent->height());
    this->setMaximumHeight(parent->height());
}
