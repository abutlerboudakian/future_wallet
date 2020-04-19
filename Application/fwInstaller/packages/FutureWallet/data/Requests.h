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
    explicit Requests(QObject * parent = nullptr, bool debug=false);

    // Inputs
    std::vector<double> * getPrediction(QString userId, QJsonObject Wages, QJsonObject Invest, QJsonObject Assets, int years);

    QJsonObject getInputs(QString userid);

    // Budgets
    bool addBudget(BudgetData * budget, QString userid);
    BudgetData * loadBudget(QString budgetId, QString userid);
    std::pair<bool, QStringList> listBudgets(QString userId);

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
    QString Location = "http://butlea2.cs.rpi.edu";
    bool debug;
};

#endif // REQUESTS_H
