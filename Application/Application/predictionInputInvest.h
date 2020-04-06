#ifndef PREDICTIONINPUTINVEST_H
#define PREDICTIONINPUTINVEST_H

#include <QWidget>
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
    void getStockData();
    unsigned int counter = 2;

private slots:
    void getAssetView();
    void getWagesView();
    void Exit();

    void addStock();
    void removeStock();
};

#endif // PREDICTIONINPUTINVEST_H
