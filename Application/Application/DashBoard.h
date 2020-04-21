#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
class Controller; // Forward declaration to resolve circular dependency
#include "Controller.h"
#include <iostream>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace Ui {
class DashBoard;
}

class DashBoard : public QWidget
{
    Q_OBJECT
    /*
     * View class for the DashBoard View
     */

public:
    explicit DashBoard(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~DashBoard();

    void updateMetrics();
    void updateMessage(QString userId);
    void updateBudget(QString budgetId);

private:
    Ui::DashBoard *ui;
    Controller * controller;
    ChartMap * metrics;

private slots:
   void getInputView();
   void getBudgetView();
   void getMetricsPieView();
   void getMetricsBarView();
   void getBudgetPieView();
   void getBudgetBarView();
   void getAccountManageView();
};

#endif // DASHBOARD_H
