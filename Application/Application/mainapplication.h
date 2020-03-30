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
    PieGUI * PieCreator; // Keeping them as ptrs for now since we need the data stored in them for chart views until we're done with the chart views
    LineGUI * LineCreator; // putting them on the stack removes the elements in the chartview leading to segfault
    BarGUI * BarCreator;
};
#endif // MAINAPPLICATION_H
