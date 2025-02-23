#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QLabel>

#include "menupage.h"

class MainWindow : public QWidget {
    Q_OBJECT

private slots:
    void paintEvent(QPaintEvent *event);//background setter
    void exitApp();
    void backToMenu();//go back to 1st page
    void respSquare();//your choise to resp object is..
    void respRectangle();//your choise to resp object is..
    void respCircle();//your choise to resp object is..
    void respTriangle();//your choise to resp object is..
    void respTrapeze();//your choise to resp object is..
    void killSquare();//delete all of this object type
    void killRectangle();//delete all of this object type
    void killCircle();//delete all of this object type
    void killTriangle();//delete all of this object type
    void killTrapeze();//delete all of this object type


public:
    MainWindow(QWidget *parent = nullptr);
private:
    QPixmap background;//Background txt
    QPushButton *exitAppButton;
    QPushButton *menuButton;//go to welcome page button
    QLabel *frame;//frame on screen(area to simulation of object phycics)
    QPushButton *respSquareButton;
    QPushButton *respRectangleButton;
    QPushButton *respCircleButton;
    QPushButton *respTriangleButton;
    QPushButton *respTrapezeButton;
    QPushButton *killSquareButton;
    QPushButton *killRectangleButton;
    QPushButton *killCircleButton;
    QPushButton *killTriangleButton;
    QPushButton *killTrapezeButton;
};

#endif // MAINWINDOW_H
