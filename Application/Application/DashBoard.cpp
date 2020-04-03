#include "DashBoard.h"

DashBoard::DashBoard(QMainWindow * parent, Controller * controller) : QWidget(parent), BaseView()
{
  this->controller = controller;
  QPushButton * Predict;
  QLabel * welcome;
  QPushButton * Budget;
  QPushButton * Pie;
  QPushButton * VBar;
  QPushButton * Line;
  QPushButton * HBar;
  QGridLayout * top;
  QGridLayout * thumbnails;
  QWidget * metrics;
  QScrollArea * metricsWrapper;

  QString height = QString("height:50px");
  Predict = new QPushButton("Predict", this);
  welcome = new QLabel("Welcome to Future Wallet", this);
  welcome->setStyleSheet(QString("background-color:orange;\nborder:1px solid black;height:50px;\nwidth:200px"));
  Budget = new QPushButton("Budget", this);

  // Set styles for the buttons
  Predict->setStyleSheet(height);
  Budget->setStyleSheet(height);

  // Metrics Area
  metricsWrapper = new QScrollArea(this);
  metrics = new QWidget(this);
  //metrics->setStyleSheet(QString("background-color:orange;\nborder:1px solid black"));
  metricsWrapper->setWidget(metrics);
  // metricsWrapper->setMaximumSize(QSize(16777215, 1677));

  // Create the buttons for the chart views
  Pie = new QPushButton("View As Pie Graph", this);
  VBar = new QPushButton("View As Vertical Bar Graph", this);
  Line = new QPushButton("View As Line Graph", this);
  HBar = new QPushButton("View As Horizontal Bar Graph", this);
  /* height = QString("width:auto;\nheight:auto"); // setting solid pixels causes resize issues
  Pie->setStyleSheet(height);
  VBar->setStyleSheet(height);
  Line->setStyleSheet(height);
  HBar->setStyleSheet(height); */

  // Throw all elements onto the gridlayout
  gridLayout = new QGridLayout(this);
  top = new QGridLayout(this);
  thumbnails = new QGridLayout(this);
  // widget, row, column, alignment
  top->addWidget(Predict, 0, 0, Qt::AlignLeft);
  top->addWidget(welcome, 0, 1, Qt::AlignCenter);
  top->addWidget(Budget, 0, 2, Qt::AlignRight);
  thumbnails->addWidget(Pie, 0, 0);
  thumbnails->addWidget(VBar, 0, 1);
  thumbnails->addWidget(Line, 0, 2);
  thumbnails->addWidget(HBar, 0, 3);
  gridLayout->addLayout(top, 0, 0);
  gridLayout->addWidget(metricsWrapper, 1, 0);
  gridLayout->addLayout(thumbnails, 2, 0);
/*
  Pie->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  VBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  Line->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  HBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
*/
  // Finish setting up the sidebar
  this->setLayout(gridLayout);

  // Set up event triggers:
  connect(Predict, SIGNAL(released()), this, SLOT(getInputView()));
  connect(Budget, SIGNAL(released()), this, SLOT(getBudgetView()));
  connect(Pie, SIGNAL(released()), this, SLOT(getPieView()));
  connect(VBar, SIGNAL(released()), this, SLOT(getVBarView()));
  connect(Line, SIGNAL(released()), this, SLOT(getLineView()));
  connect(HBar, SIGNAL(released()), this, SLOT(getHBarView()));
}

// Delegate destruction of the other elements to the base QWidget destructor
DashBoard::~DashBoard()
{
  delete gridLayout;
}


// Slots -- aka signals
void DashBoard::getInputView()
{
    std::cout<<"HelloWorld"<<std::endl;
    return;
}

void DashBoard::getBudgetView()
{
    std::cout<<"HelloWorld"<<std::endl;
    return;
}

void DashBoard::getPieView()
{
    std::cout<<"HelloWorld"<<std::endl;
    return;
}

void DashBoard::getVBarView()
{
    std::cout<<"HelloWorld"<<std::endl;
    return;
}

void DashBoard::getLineView()
{
    std::cout<<"HelloWorld"<<std::endl;
    return;
}

void DashBoard::getHBarView()
{
    std::cout<<"HelloWorld"<<std::endl;
    return;
}
