#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QMainWindow>
#include "DashBoard.h"
#include "LaunchMenu.h"
#include <memory>
#include <QStackedWidget>

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
};
#endif // MAINAPPLICATION_H
