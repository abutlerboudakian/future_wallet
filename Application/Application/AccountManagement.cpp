#include "AccountManagement.h"
#include "ui_AccountManagement.h"

AccountManagement::AccountManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountManagement)
{
    ui->setupUi(this);
}

AccountManagement::~AccountManagement()
{
    delete ui;
}

/* Function to set the controller and event triggers for the view
 * @modifies this->controller
 * @effects this->controller = controller
 */
void AccountManagement::setController(Controller * controller)
{
    this->controller = controller;

    connect(ui->cancelUpdateAccount, SIGNAL(released()), this, SLOT(Exit()));
    connect(ui->UpdateAccount, SIGNAL(released()), this, SLOT(Create()));
}

/* Function that closes the modal
 */
void AccountManagement::Exit() {
    this->controller->closeAccountManage();
    return;
}

/* Function that updates user info and closes the modal
 */
void AccountManagement::Update() {
    // New info
    std::string newPass = ui->newPassEdit->text().toStdString();
    std::string newPassConfirm = ui->confirmNewPassEdit->text().toStdString();
    std::string newUser = ui->changeUserEdit->text().toStdString();

    // Old info
    // QString oldUser = this->controller->userid;

    if(newUser.length() != 0 ) {
        // Should update with old password
        this->controller->UpdateUserInfo(QString::fromStdString(newUser), QString::fromStdString(newPass));
    }
    if(newPass.length() != 0) {
        if(newPass.compare(newPassConfirm)) {
            this->controller->UpdateUserInfo(QString::fromStdString(newUser), QString::fromStdString(newPass));
        } else {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Passwords do not match.");
            messageBox.setFixedSize(500,200);
            return;
        }
    }
    AccountManagement::Exit();
}
