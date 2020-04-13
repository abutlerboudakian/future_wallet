#include "mainapplication.h"
#include <memory>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller * mainController = new Controller;
    MainApplication w;                               // Create QMainWindow
    w.setController(mainController);
    mainController->setMain(&w);
    w.CreateViews(mainController);                   // Create All Views
    mainController->setViews(w.getViews());          // Give Controller the views
    w.showMaximized();
    return a.exec();
}
