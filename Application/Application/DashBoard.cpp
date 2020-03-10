#include "DashBoard.h"

DashBoard::DashBoard(QMainWindow * parent) : QWidget(parent)
{
  Predict = new QPushButton("Predict");
  Budget = new QPushButton("Budget");
  Pie = new QPushButton("View As Pie Graph");
  VBar = new QPushButton("View As Vertical Bar Graph");
  Line = new QPushButton("View As Line Graph");
  HBar = new QPushButton("View As Horizontal Bar Graph");

  welcome = new QLabel("Welcome to Future Wallet");

  metrics = new QWidget(parent);
  metrics->setStyleSheet(QString("background-color:orange;\nborder:1px solid black"));

  // Throw all elements onto the gridlayout
  gridLayout = new QGridLayout(parent);
  thumbnails = new QHBoxLayout(parent);
  gridLayout->addWidget(Predict, 0, 0, Qt::AlignLeft);
  gridLayout->addWidget(welcome, 0, 1, Qt::AlignCenter);
  gridLayout->addWidget(Budget, 0, 2, Qt::AlignRight);
  gridLayout->addWidget(metrics, 1, 0, Qt::AlignCenter);
  gridLayout->addLayout(thumbnails, 2, 0, Qt::AlignCenter);
  thumbnails->addWidget(Pie);
  thumbnails->addWidget(VBar);
  thumbnails->addWidget(Line);
  thumbnails->addWidget(HBar);

  Pie->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  VBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  Line->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  HBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

  // Finish setting up the sidebar
  this->setLayout(gridLayout);
}

DashBoard::~DashBoard()
{
    delete Predict;
    delete welcome;
    delete Budget;
    delete Pie;
    delete VBar;
    delete Line;
    delete HBar;
    delete gridLayout;
    delete thumbnails;
    delete metrics;
}
