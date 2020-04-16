#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

class Controller;

#include <QDialog>
#include "Controller.h"

namespace Ui {
class AccountManagement;
}

class AccountManagement : public QDialog
{
    Q_OBJECT

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
