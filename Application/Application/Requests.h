#ifndef REQUESTS_H
#define REQUESTS_H

#include <QObject>
#include <QEventLoop>

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

class Requests : public QObject
{
    Q_OBJECT
    /* Class made to communicate with server controller
     */
public:
    explicit Requests(QObject * parent = nullptr);

    void getPrediction(...);
    //void getPredictionCallback(...);

    void getInputs(...);
    //void getInputsCallback(...);

    void addBudget(...);
    //void addBudgetCallback(...);

    void loadBudget(...);
    //void loadBudgetCallback(...);

    void listBudgets(...);
    //void listBudgetsCallback(...);

    void login(...);
    //void loginCallback(...);

    void logout(...);
    //void logoutCallback(...);

    void getIndustries(...);
    //void getIndustriesCallback(...);

private slots:
    void readData(QNetworkReply * reply);
private:
    QNetworkAccessManager NAMSender;
    QByteArray Data;
};

#endif // REQUESTS_H
