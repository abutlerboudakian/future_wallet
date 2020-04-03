#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QWidget>
#include "BaseView.h"

namespace Ui {
class BudgetPage;
}

class BudgetPage : public QWidget, public BaseView
{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~BudgetPage();

private:
    Ui::BudgetPage *ui;

private slots:
    void getLoadBudgetView();
    void getCreateBudgetView();
};

#endif // BUDGETPAGE_H
