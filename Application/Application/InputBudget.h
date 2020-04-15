#ifndef INPUTBUDGET_H
#define INPUTBUDGET_H

#include <QWidget>

namespace Ui {
class InputBudget;
}

class InputBudget : public QWidget
{
    Q_OBJECT

public:
    explicit InputBudget(QWidget *parent = nullptr);
    ~InputBudget();

private:
    Ui::InputBudget *ui;
};

#endif // INPUTBUDGET_H
