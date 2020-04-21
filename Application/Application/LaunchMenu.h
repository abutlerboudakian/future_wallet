#ifndef LAUNCHMENU_H
#define LAUNCHMENU_H

#include <QWidget>
class Controller; // Forward declaration to resolve circular dependency
#include "Controller.h"
#include <iostream>

namespace Ui {
class LaunchMenu;
}

class LaunchMenu : public QWidget
{
    Q_OBJECT
    /*
     * View class for the login View
     */

public:
    explicit LaunchMenu(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~LaunchMenu();

private:
    Ui::LaunchMenu *ui;
    Controller * controller;

private slots:
    void runLogin();
    void runCreateAccount();
};

#endif // LAUNCHMENU_H
