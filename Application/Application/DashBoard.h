#ifndef DASHBOARDPAGE
#define DASHBOARDPAGE

#include <QGridLayout>
#include <QLabel>
//#include <QHBoxLayout>
#include <QPushButton>
#include <QMainWindow>
#include <QSizePolicy>
#include <QScrollArea>
#include <memory>

class DashBoard : public QWidget
{
  Q_OBJECT 
  public:
    explicit DashBoard(QMainWindow * parent);
    ~DashBoard();
  private:
    QPushButton * Predict;
    QLabel * welcome;
    QPushButton * Budget;
    QPushButton * Pie;
    QPushButton * VBar;
    QPushButton * Line;
    QPushButton * HBar;
    QGridLayout * gridLayout;
    QGridLayout * top;
    QGridLayout * thumbnails;
    QWidget * metrics;
    QScrollArea * metricsWrapper;

  private slots:
    void getInputView();
    void getBudgetView();
    void getPieView();
    void getVBarView();
    void getLineView();
    void getHBarView();
};

#endif
