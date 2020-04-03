#include "mainapplication.h"
#include <memory>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller * mainController = new Controller;
    MainApplication w; // Create all the views
    w.CreateViews(mainController);
    mainController->setViews(w.getViews());
    w.showMaximized();
    return a.exec();
}
