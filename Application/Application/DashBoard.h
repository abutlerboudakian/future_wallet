#ifndef DASHBOARDPAGE
#define DASHBOARDPAGE

#include "mainapplication.h"
#include "BaseView.h"
#include <iostream>

#include <QGridLayout>
#include <QLabel>
//#include <QHBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QScrollArea>
#include <memory>

class DashBoard : public QWidget, public BaseView
{
  Q_OBJECT 
  public:
    explicit DashBoard(QMainWindow * parent, Controller * controller);
    ~DashBoard();
  private:
    /*QPushButton * Predict;
    QLabel * welcome;
    QPushButton * Budget;
    QPushButton * Pie;
    QPushButton * VBar;
    QPushButton * Line;
    QPushButton * HBar;
    QWidget * metrics;
    QScrollArea * metricsWrapper;*/
    QGridLayout * gridLayout;
    QGridLayout * top;
    QGridLayout * thumbnails;

    //Controller * controller;

  private slots:
    void getInputView();
    void getBudgetView();
    void getPieView();
    void getVBarView();
    void getLineView();
    void getHBarView();
};

#endif
