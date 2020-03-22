#ifndef LAUNCHMENU_H
#define LAUNCHMENU_H

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

class LaunchMenu : public QWidget
{
    Q_OBJECT
    public:
      explicit LaunchMenu(QMainWindow * parent);
      ~LaunchMenu();
    private:
      QLabel* TitleBar;
      QGridLayout* MainLayout;
      QFormLayout* Form;
      QGridLayout* Buttons;
      QPushButton* CreateAccount;
      QPushButton* Login;

    private slots:
      void runLogin();
      void runCreateAccount();
};

#endif // LAUNCHMENU_H
