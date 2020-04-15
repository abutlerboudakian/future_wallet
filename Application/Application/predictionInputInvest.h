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
    void getStockData();
    QJsonObject toJSON();
    void fromJson(QJsonObject savedData);

private:
    Ui::predictionInputInvest *ui;
    Controller * controller;

    unsigned int counter = 2;

    QDoubleValidator * validDouble;

private:
    void setupValidator();


private slots:
    void getAssetView();
    void getWagesView();
    void Exit();

    void addStock();
    void addStock(QString name, double shares);
    void removeStock();
};

#endif // PREDICTIONINPUTINVEST_H
