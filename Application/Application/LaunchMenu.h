#ifndef LAUNCHMENU_H
#define LAUNCHMENU_H

#include "mainapplication.h"
#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>

class LaunchMenu : public QWidget
{
    Q_OBJECT
    public:
      explicit LaunchMenu(QMainWindow * parent, Controller * controller);
      ~LaunchMenu();
    private:
      QLabel* TitleBar;
      QGridLayout* MainLayout;
      QFormLayout* Form;
      QGridLayout* Buttons;
      QPushButton* CreateAccount;
      QPushButton* Login;
      QLineEdit* UserLineEdit;
      QLineEdit* PassLineEdit;

      Controller * controller;

    private slots:
      void runLogin();
      void runCreateAccount();
};

#endif // LAUNCHMENU_H
