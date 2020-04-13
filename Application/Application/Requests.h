#ifndef REQUESTS_H
#define REQUESTS_H

#include <QObject>
#include <QEventLoop>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

#include <QCryptographicHash>

#include "BudgetData.h"

class Requests : public QObject
{
    Q_OBJECT
    /* Class made to communicate with server controller
     */
public:
    explicit Requests(QObject * parent = nullptr);

    // Inputs
    std::vector<double> * getPrediction(...);

    void getInputs(...);

    // Budgets
    bool addBudget(BudgetData * budget, QString userid);
    BudgetData * loadBudget(QString budgetId, QString userid);
    QStringList listBudgets(QString userId);

    // Auth
    bool login(QString userId, QString Password);

    void logout(QString userId);

    bool Register(QString userId, QString Password);
    bool UpdateUserInfo(QString OriginalUserId, QString userId, QString Password);

    // Aux
    QStringList getIndustries();
    QStringList getStocks();

private slots:
    void readData(QNetworkReply * reply);
private:
    QNetworkAccessManager NAMSender;
    QByteArray Data;
    QString Location = "http://127.0.0.1:5000";
};

#endif // REQUESTS_H
