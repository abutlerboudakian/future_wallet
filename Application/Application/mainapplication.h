#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include "Controller.h"
#include <QMainWindow>
#include <QStackedWidget>
#include "DashBoard.h"
#include "LaunchMenu.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainApplication; }
QT_END_NAMESPACE

class MainApplication : public QMainWindow
{
    Q_OBJECT

public:
    MainApplication(QWidget *parent = nullptr);
    ~MainApplication();
    void CreateViews(Controller * maincontroller);
    QStackedWidget * getViews();

private:
    Ui::MainApplication *ui;
    QStackedWidget * Views;
};
#endif // MAINAPPLICATION_H
