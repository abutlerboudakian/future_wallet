#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include "BaseView.h"
#include <iostream>

namespace Ui {
class DashBoard;
}

class DashBoard : public QWidget, public BaseView
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr, Controller * controller = nullptr);
    ~DashBoard();

private:
    Ui::DashBoard *ui;
    void update();

private slots:
   void getInputView();
   void getBudgetView();
   void getPieView();
   void getVBarView();
   void getLineView();
};

#endif // DASHBOARD_H
