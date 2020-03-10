#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QMainWindow>
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

private:
    Ui::MainApplication *ui;
    std::unique_ptr<QWidget> Content; // The state is any Widget we use to display content
};
#endif // MAINAPPLICATION_H
