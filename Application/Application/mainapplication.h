#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

class Controller;
#include "Controller.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainApplication; }
QT_END_NAMESPACE

class MainApplication : public QMainWindow
{
    Q_OBJECT
    /*
     * Class for the main window for our application.
     */

public:
    MainApplication(QWidget *parent = nullptr);
    ~MainApplication();
    void CreateViews(Controller * maincontroller);
    QStackedWidget * getViews();
    void setController(Controller * controller);

protected:
    virtual void closeEvent(QCloseEvent * event) override;

private:
    Ui::MainApplication *ui;
    QStackedWidget * Views;
    Controller * controller;

private slots:
    void getUserAccountView();
    void logout();
};
#endif // MAINAPPLICATION_H
