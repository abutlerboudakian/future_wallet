#ifndef INPUTBUDGET_H
#define INPUTBUDGET_H

class Controller;
#include "Controller.h"

#include <QWidget>
#include <QLabel>
#include <QSignalMapper>
#include <QSlider>

namespace Ui {
class InputBudget;
}

class InputBudget : public QWidget
{
    Q_OBJECT

public:
    explicit InputBudget(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~InputBudget();

private:
    Ui::InputBudget *ui;
    Controller * controller;
    QSignalMapper * signalMapper;

    void getCategoryData();
    // Should have 1 category for budget, for example "100% of my budget is dedicated to collecting empty danimal bottles"
    unsigned int counter = 1;

private slots:
    void addCategory();
    void removeCategory();
    void Create();
    void Exit();
    void updateLabel(QSlider * slider, QLabel *label);
};

#endif // INPUTBUDGET_H
