#include "mainapplication.h"
#include "ui_mainapplication.h"

MainApplication::MainApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainApplication)
{
    ui->setupUi(this);

    content = new DashBoard(this);
    setCentralWidget(content);
    content->show();
}

MainApplication::~MainApplication()
{
    delete ui;
    delete content;
}

