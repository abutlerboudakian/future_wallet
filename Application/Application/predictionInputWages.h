#ifndef PREDICTIONINPUTWAGES_H
#define PREDICTIONINPUTWAGES_H

#include <QGridLayout>
#include <QLabel>
//#include <QHBoxLayout>
#include <QPushButton>
#include <QMainWindow>
#include <QSizePolicy>
#include <QScrollArea>
#include <memory>


class predictionInputWages: public QWidget
{
public:
    Q_OBJECT
    public:
      explicit predictionInputWages(QMainWindow * parent);
      ~predictionInputWages();
};

#endif // PREDICTIONINPUTWAGES_H
