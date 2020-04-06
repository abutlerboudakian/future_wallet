#ifndef PREDICTIONINPUTINVEST_H
#define PREDICTIONINPUTINVEST_H

#include <QWidget>
#include <QValidator>
#include <QDoubleValidator>
class Controller;
#include "Controller.h"

namespace Ui {
class predictionInputInvest;
}

class predictionInputInvest : public QWidget
{
    Q_OBJECT

public:
    explicit predictionInputInvest(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~predictionInputInvest();

private:
    Ui::predictionInputInvest *ui;
    Controller * controller;
    QDoubleValidator * validDouble;
    QIntValidator * validInt;

private:
    void setupValidator();

private slots:
    void getAssetView();
    void getWagesView();
    void Exit();
};

#endif // PREDICTIONINPUTINVEST_H
