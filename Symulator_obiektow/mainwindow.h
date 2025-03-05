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
#include <QTimer>
#include <memory>
#include <vector>
#include <QLineEdit>
#include <map>//to keep phycisal values of objects(as pair of object type and value of phycisal type)

#include "menupage.h"
#include "threadedwindpoint.h"

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
    void respWindPoint(int, int);//resp wind object based on circle rxr
    void killSquare();//delete all of this object type
    void killRectangle();//delete all of this object type
    void killCircle();//delete all of this object type
    void killTriangle();//delete all of this object type
    void killTrapeze();//delete all of this object type
    void killWindPoints();//delete all of this object type
    void mousePressEvent(QMouseEvent *event);//mouse event handling(here to resp object on click place)
    void updateSimulation();//event which refresh scene
public:
    MainWindow(QWidget *parent = nullptr);//constructor
    ~MainWindow();//destructor

    //is object type button clicked(object selected? )
    bool squareCondition=false;
    bool rectangleCondition=false;
    bool circleConditon=false;
    bool triangleCondition=false;
    bool trapezeCondition=false;
    bool windPointCondition=false;

    bool changeSquareCondition();
    bool changeRectangleCondition();
    bool changeCircleConditon();
    bool changeTriangleCondition();
    bool changeTrapezeCondition();
    bool changeWindPointCondition();

private:
    QPixmap background;//Background txt
    QPushButton *exitAppButton;
    QPushButton *menuButton;//go to welcome page button
    QLabel *frame;//frame on screen(area to simulation of object phycics)
    QLabel *saveAreaFrame;//area where object can be created on the simulation beginning
    QGraphicsScene *scene;//scene for simulation
    QGraphicsView *view;//view for scene
    QTimer *simulationTimer;//timer to refresh scene

    map<QGraphicsItem*, QLineEdit*> massInputs;
    map<QGraphicsItem*, QLineEdit*> frictionInputs;

    //buttons for resp or delete object
    QPushButton *respSquareButton;
    QPushButton *respRectangleButton;
    QPushButton *respCircleButton;
    QPushButton *respTriangleButton;
    QPushButton *respTrapezeButton;
    QPushButton *respWindPointButton;
    QPushButton *killSquareButton;
    QPushButton *killRectangleButton;
    QPushButton *killCircleButton;
    QPushButton *killTriangleButton;
    QPushButton *killTrapezeButton;
    QPushButton *killWindPointButton;

    //frame area checker
    bool isWithinFrame(int x, int y);//method to check if position of mouse coursor is inside frame(-50px inside of area)
    int x_frame=1366;
    int y_frame=663;
    int save_gap=30;//px amount from frame to keep area save to resp on the beginning by mouse click
    int frame_siz=10;//10 px is frame width

    void setPhysicalProperties(QGraphicsItem* item);//set physical values(mass and friction) to object
    QLineEdit *massInput;//to type in mass of the object
    QLineEdit *frictionInput;//to type in friction input
};

#endif // MAINWINDOW_H
