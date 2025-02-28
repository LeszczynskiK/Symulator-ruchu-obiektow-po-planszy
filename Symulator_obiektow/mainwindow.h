#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMessageBox>

#include "menupage.h"

class MainWindow : public QWidget {
    Q_OBJECT

private slots:
    void paintEvent(QPaintEvent *event);//background setter
    void exitApp();
    void backToMenu();//go back to 1st page
    void respSquare(int,int);//your choise to resp object is..
    void respRectangle(int,int);//your choise to resp object is..
    void respCircle(int,int);//your choise to resp object is..
    void respTriangle(int,int);//your choise to resp object is..
    void respTrapeze(int,int);//your choise to resp object is..
    void killSquare();//delete all of this object type
    void killRectangle();//delete all of this object type
    void killCircle();//delete all of this object type
    void killTriangle();//delete all of this object type
    void killTrapeze();//delete all of this object type
    void mousePressEvent(QMouseEvent *event);//mouse event handling(here to resp object on click place)

public:
    MainWindow(QWidget *parent = nullptr);

    //is object type button clicked(object selected? )
    bool squareCondition=false;
    bool rectangleCondition=false;
    bool circleConditon=false;
    bool triangleCondition=false;
    bool trapezeCondition=false;

    bool changeSquareCondition();
    bool changeRectangleCondition();
    bool changeCircleConditon();
    bool changeTriangleCondition();
    bool changeTrapezeCondition();

private:
    QPixmap background;//Background txt
    QPushButton *exitAppButton;
    QPushButton *menuButton;//go to welcome page button
    QLabel *frame;//frame on screen(area to simulation of object phycics)
    QLabel *saveAreaFrame;//area where object can be created on the simulation beginning
    QGraphicsScene *scene;//scene for simulation
    QGraphicsView *view;//view for scene

    //buttons for resp or delete object
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

    //frame area checker
    bool isWithinFrame(int x, int y);//method to check if position of mouse coursor is inside frame(-50px inside of area)
    int x_frame=1366;
    int y_frame=663;
    int save_gap=30;//px amount from frame to keep area save to resp on the beginning by mouse click
    int frame_siz=10;//10 px is frame width
};

#endif // MAINWINDOW_H
