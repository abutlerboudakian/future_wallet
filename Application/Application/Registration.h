#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
class Controller;
#include "Controller.h"

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT
    /*
     * View class for the Registration View
     */

public:
    explicit Registration(QWidget *parent = nullptr, Controller *controller = nullptr);
    ~Registration();

    void clear();

private:
    Ui::Registration *ui;
    Controller *controller;

private slots:
    void registerUser();
    void cancelRegistration();
};

#endif // REGISTRATION_H
