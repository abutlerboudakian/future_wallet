/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnPieChart;
    QPushButton *btnChangeGraph;
    QTextBrowser *txtMetricOut;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1036, 618);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btnPieChart = new QPushButton(centralwidget);
        btnPieChart->setObjectName(QString::fromUtf8("btnPieChart"));
        btnPieChart->setGeometry(QRect(170, 40, 131, 41));
        btnChangeGraph = new QPushButton(centralwidget);
        btnChangeGraph->setObjectName(QString::fromUtf8("btnChangeGraph"));
        btnChangeGraph->setGeometry(QRect(170, 120, 131, 41));
        txtMetricOut = new QTextBrowser(centralwidget);
        txtMetricOut->setObjectName(QString::fromUtf8("txtMetricOut"));
        txtMetricOut->setGeometry(QRect(495, 1, 541, 571));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1036, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnPieChart->setText(QCoreApplication::translate("MainWindow", "Pie Chart", nullptr));
        btnChangeGraph->setText(QCoreApplication::translate("MainWindow", "View Est. Change", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
