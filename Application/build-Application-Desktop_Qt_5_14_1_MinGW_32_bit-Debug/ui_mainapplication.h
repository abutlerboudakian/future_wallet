/********************************************************************************
** Form generated from reading UI file 'mainapplication.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINAPPLICATION_H
#define UI_MAINAPPLICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainApplication
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainApplication)
    {
        if (MainApplication->objectName().isEmpty())
            MainApplication->setObjectName(QString::fromUtf8("MainApplication"));
        MainApplication->resize(800, 600);
        centralwidget = new QWidget(MainApplication);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainApplication->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainApplication);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        MainApplication->setMenuBar(menubar);
        statusbar = new QStatusBar(MainApplication);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainApplication->setStatusBar(statusbar);

        retranslateUi(MainApplication);

        QMetaObject::connectSlotsByName(MainApplication);
    } // setupUi

    void retranslateUi(QMainWindow *MainApplication)
    {
        MainApplication->setWindowTitle(QCoreApplication::translate("MainApplication", "MainApplication", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainApplication: public Ui_MainApplication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINAPPLICATION_H
