#ifndef PREDICTIONINPUTASSETS_H
#define PREDICTIONINPUTASSETS_H

#include <QWidget>
#include <QDoubleValidator>
class Controller; // Forward declaration to resolve circular dependency
#include "Controller.h"

namespace Ui {
class predictionInputAssets;
}

class predictionInputAssets : public QWidget
{
    Q_OBJECT

public:
    explicit predictionInputAssets(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~predictionInputAssets();
    QJsonObject toJSON();
    void fromJson(QJsonObject savedData);

private:
    Ui::predictionInputAssets *ui;
    Controller * controller;

    void getResidenceData();
    void getRentalData();
    unsigned int resCounter = 2;
    unsigned int rentCounter = 2;

    QDoubleValidator * validDouble;
    QIntValidator * validInt;

private:
    void setupValidator();


private slots:
    void addResidence();
    void addResidence(QString location, double value);
    void addRental();
    void addRental(QString location, double value);
    void removeResidence();
    void removeRental();
    void submitInputs();
    void getInvestView();
    void Exit();
};

#endif // PREDICTIONINPUTASSETS_H
