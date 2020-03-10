#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QMainWindow>

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
};
#endif // MAINAPPLICATION_H
