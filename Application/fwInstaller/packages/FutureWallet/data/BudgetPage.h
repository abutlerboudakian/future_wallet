#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

class Controller; // Forward declaration to patch circular dependency

#include <QDialog>
#include "Controller.h"

namespace Ui {
class BudgetPage;
}

class BudgetPage : public QDialog
{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);
    ~BudgetPage();
    void setController(Controller * controller);
    void updateUserList();

protected:
    virtual void closeEvent(QCloseEvent * event) override;

private:
    Ui::BudgetPage *ui;
    Controller * controller;

private slots:
    void getLoadBudgetView();
    void getCreateBudgetView();

    void changeSelected();
    //void closure();
};

#endif // BUDGETPAGE_H
