#include "mainapplication.h"
#include <memory>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::unique_ptr<MainApplication> w (new MainApplication);
    w->show();
    return a.exec();
}
