#include "mainwindow.h"

#include <QApplication> // Handles event handling and mouse movements, etc.
#include <QDebug> // For debugging output

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec(); // Puts application into loop for diff events in application to be handled
}
