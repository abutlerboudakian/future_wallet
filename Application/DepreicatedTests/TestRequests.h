#ifndef TESTREQUESTS_H
#define TESTREQUESTS_H

#include <QtTest>
#include <QCoreApplication>
#include "../Application/Requests.h"

class TestRequests : public QObject
{
    Q_OBJECT

public:
    TestRequests();
    ~TestRequests();

// Unit Tests are slot functions
private slots:
    // Inputs
    void TestGetPrediction();
    /*void TestGetInputs();
    void TestAddBudget();
    void TestLoadBudget();
    void TestListBudgets();
    void TestLogin();
    void TestLogout();
    void TestRegister();
    void TestUpdateUserInfo();
    void TestGetIndustries();
    void TestRegister();
    void TestUpdateUserInfo();*/

};

#endif // TESTREQUESTS_H
