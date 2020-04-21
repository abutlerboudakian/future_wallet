#ifndef ACCOUNTMANAGEMENT_H
#define ACCOUNTMANAGEMENT_H

#include <QDialog>
class Controller;
#include "Controller.h"

namespace Ui {
class AccountManagement;
}

class AccountManagement : public QDialog
{
    Q_OBJECT
    /*
     * View class for the User Account Management View
     */

public:
    explicit AccountManagement(QWidget *parent = nullptr);
    ~AccountManagement();
    void setController(Controller * controller);

private:
    Ui::AccountManagement *ui;
    Controller * controller;

private slots:
    void Exit();
    void Update();
};

#endif // ACCOUNTMANAGEMENT_H
