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

public:
    explicit DashBoard(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~DashBoard();

    void updateMetrics();
    void updateBudget();

private:
    Ui::DashBoard *ui;
    Controller * controller;

private slots:
   void getInputView();
   void getBudgetView();
   void getMetricsPieView();
   void getMetricsBarView();
   void getMetricsLineView();
   void getBudgetPieView();
   void getBudgetBarView();
};

#endif // DASHBOARD_H
