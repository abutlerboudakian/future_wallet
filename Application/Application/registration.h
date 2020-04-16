#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
class Controller;
#include "Controller.h"

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr, Controller *controller = nullptr);
    ~Registration();

private:
    Ui::Registration *ui;
    Controller *controller;

private slots:
    void registerUser();
    void cancelRegistration();
};

#endif // REGISTRATION_H
