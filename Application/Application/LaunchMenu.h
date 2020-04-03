#ifndef LAUNCHMENU_H
#define LAUNCHMENU_H

#include "mainapplication.h"
#include "BaseView.h"
#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>

class LaunchMenu : public QWidget, public BaseView
{
    Q_OBJECT
    public:
      explicit LaunchMenu(QMainWindow * parent, Controller * controller);
      ~LaunchMenu();
    private:
      QGridLayout* MainLayout;
      /* QLabel* TitleBar;
      QPushButton* CreateAccount;
      QPushButton* Login;
      QLineEdit* UserLineEdit;
      QLineEdit* PassLineEdit; */
      QFormLayout* Form;
      QGridLayout* Buttons;

      //Controller * controller;

    private slots:
      void runLogin();
      void runCreateAccount();
};

#endif // LAUNCHMENU_H
