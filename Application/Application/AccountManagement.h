#ifndef ACCOUNTMANAGEMENT_H
#define ACCOUNTMANAGEMENT_H

#include <QDialog>

namespace Ui {
class AccountManagement;
}

class AccountManagement : public QDialog
{
    Q_OBJECT

public:
    explicit AccountManagement(QWidget *parent = nullptr);
    ~AccountManagement();

private:
    Ui::AccountManagement *ui;
};

#endif // ACCOUNTMANAGEMENT_H
