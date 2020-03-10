#include "mainapplication.h"
#include <memory>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainApplication w;
    w.showMaximized();
    return a.exec();
}
