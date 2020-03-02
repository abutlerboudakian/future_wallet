#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget> // For the sidebar, aka widget
#include <QPushButton> // For buttons
#include <QVBoxLayout> // For a vertical sidebar
// #include <QGridLayout> // For a grid sidebar layout
#include <QPalette> // To change bg color
#include <QColor> // For my darkslateblue

// https://stackoverflow.com/questions/44317029/designing-a-qt-creators-sidebar-in-c

class sideBar : public QWidget
{
    Q_OBJECT
  public:
    explicit sideBar(QWidget * parent = nullptr);
    void updateHeight(QWidget * parent);
  private:
};

#endif // SIDEBAR_H
