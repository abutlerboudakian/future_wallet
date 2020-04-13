#ifndef INPUTBUDGET_H
#define INPUTBUDGET_H

class Controller;
#include "Controller.h"

#include <QWidget>
#include <QLabel>

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
    Controller * controller;

    void getCategoryData();
    unsigned int counter = 1;

private slots:
    void addCategory();
    void removeCategory();
    void Create();
    void Exit();
    void updateLabel(QSlider * slider, QLabel *label);
};

#endif // INPUTBUDGET_H
