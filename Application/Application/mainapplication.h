#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QMainWindow>
#include "DashBoard.h"
#include "LaunchMenu.h"
#include <memory>
#include <QStackedWidget>

#include "ChartTemplate.h"

enum Views
{
    Login = 0,
    Dashboard = 1
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainApplication; }
QT_END_NAMESPACE

class MainApplication : public QMainWindow
{
    Q_OBJECT

public:
    MainApplication(QWidget *parent = nullptr);
    ~MainApplication();

private:
    Ui::MainApplication *ui;
    QStackedWidget * content;
    PieGUI * PieCreator;
};
#endif // MAINAPPLICATION_H
