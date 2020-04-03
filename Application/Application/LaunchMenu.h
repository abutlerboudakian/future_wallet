#ifndef LAUNCHMENU_H
#define LAUNCHMENU_H

#include <QWidget>
#include "BaseView.h"
#include <iostream>

namespace Ui {
class LaunchMenu;
}

class LaunchMenu : public QWidget, public BaseView
{
    Q_OBJECT

public:
    explicit LaunchMenu(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~LaunchMenu();

private:
    Ui::LaunchMenu *ui;

private slots:
    void runLogin();
    void runCreateAccount();
};

#endif // LAUNCHMENU_H
