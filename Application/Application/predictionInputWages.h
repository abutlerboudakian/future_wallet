#ifndef PREDICTIONINPUTWAGES_H
#define PREDICTIONINPUTWAGES_H

#include <QWidget>
#include <QValidator>
#include <QDoubleValidator>
class Controller;
#include "Controller.h"

namespace Ui {
class predictionInputWages;
}

class predictionInputWages : public QWidget
{
    Q_OBJECT

public:
    explicit predictionInputWages(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~predictionInputWages();

private:
    Ui::predictionInputWages *ui;
    Controller * controller;
    QDoubleValidator * validDouble;
    QIntValidator * validInt;

private:
    void setupValidator();

private slots:
    void getInvestView();
    void Exit();
};

#endif // PREDICTIONINPUTWAGES_H
