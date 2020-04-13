#ifndef MENU_H
#define MENU_H

class Controller;
#include "Controller.h"
#include <QMenuBar>

class Menu : public QMenuBar
{
    // The Menubar for the user
    Q_OBJECT
public:
    explicit Menu(QWidget * parent = nullptr, Controller * controller = nullptr);
    ~Menu();
    void setUserName(QString userId);

private:
    QMenu * MainTab;
    Controller * controller;

private slots:
    void getUserAccountView();
    void logout();
};

#endif // MENU_H
