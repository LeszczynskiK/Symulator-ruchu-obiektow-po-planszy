#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Shakes physics simulation");

    const int x =1366;//x window size
    const int y=768;//y window size

    setFixedSize(x, y);//Set window size

    //Background image
    background = QPixmap("/home/krzysiek89/Desktop/QT_aplikacje/Symulator_ruchu_obiektow/Symulator_obiektow/AppPage.jpg").scaled(x, y);

    //initialise scene and views
    scene = new QGraphicsScene(0, 0, 1366, 663, this);
    view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, 1366, 663);
    view->setStyleSheet("background: transparent");
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Frame based on QLabel(frame with transparent background and "frame" around)
    frame = new QLabel(this);
    frame->setGeometry(0, 0, 1366, 663);
    frame->setStyleSheet("background-color: transparent; border: 10px solid black;");

    QFont font;
    font.setPointSize(16);//Font size

    //Initialize buttons
    int x_pos=5;
    int gap=10;
    int y_pos = 670;
    int y_siz =40;
    int x_size=145;

    menuButton = new QPushButton("Menu", this);//go to main menu
    menuButton->setFont(font);
    menuButton->setGeometry(x_pos, y_pos, x_size, y_siz);
    menuButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::backToMenu);//if clicked, go back to menu page

    exitAppButton = new QPushButton("Exit", this);//exit app
    exitAppButton->setFont(font);
    exitAppButton->setGeometry(x_pos, y_pos+y_siz+gap, x_size, y_siz);
    exitAppButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(exitAppButton, &QPushButton::clicked, this, &MainWindow::exitApp);

    respSquareButton = new QPushButton("Square", this);
    respSquareButton->setFont(font);
    respSquareButton->setGeometry(x_pos+gap+2*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respSquareButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respSquareButton, &QPushButton::clicked, this, &MainWindow::respSquare);

    respRectangleButton = new QPushButton("Rectangle", this);
    respRectangleButton->setFont(font);
    respRectangleButton->setGeometry(x_pos+2*gap+3*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respRectangleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respRectangleButton, &QPushButton::clicked, this, &MainWindow::respRectangle);

    respCircleButton = new QPushButton("Circle", this);
    respCircleButton->setFont(font);
    respCircleButton->setGeometry(x_pos+3*gap+4*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respCircleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respCircleButton, &QPushButton::clicked, this, &MainWindow::respCircle);

    respTriangleButton = new QPushButton("Triangle", this);
    respTriangleButton->setFont(font);
    respTriangleButton->setGeometry(x_pos+4*gap+5*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respTriangleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respTriangleButton, &QPushButton::clicked, this, &MainWindow::respTriangle);

    respTrapezeButton = new QPushButton("Trapeze", this);
    respTrapezeButton->setFont(font);
    respTrapezeButton->setGeometry(x_pos+5*gap+6*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respTrapezeButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respTrapezeButton, &QPushButton::clicked, this, &MainWindow::respTrapeze);

    killSquareButton = new QPushButton("Delete all", this);
    killSquareButton->setFont(font);
    killSquareButton->setGeometry(x_pos+gap+2*x_size, y_pos, x_size, y_siz);
    killSquareButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(killSquareButton, &QPushButton::clicked, this, &MainWindow::killSquare);

    killRectangleButton = new QPushButton("Delete all", this);
    killRectangleButton->setFont(font);
    killRectangleButton->setGeometry(x_pos+2*gap+3*x_size, y_pos, x_size, y_siz);
    killRectangleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(killRectangleButton, &QPushButton::clicked, this, &MainWindow::killRectangle);

    killCircleButton = new QPushButton("Delete all", this);
    killCircleButton->setFont(font);
    killCircleButton->setGeometry(x_pos+3*gap+4*x_size, y_pos, x_size, y_siz);
    killCircleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(killCircleButton, &QPushButton::clicked, this, &MainWindow::killCircle);

    killTriangleButton = new QPushButton("Delete all", this);
    killTriangleButton->setFont(font);
    killTriangleButton->setGeometry(x_pos+4*gap+5*x_size, y_pos, x_size, y_siz);
    killTriangleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(killTriangleButton, &QPushButton::clicked, this, &MainWindow::killTriangle);

    killTrapezeButton = new QPushButton("Delete all", this);
    killTrapezeButton->setFont(font);
    killTrapezeButton->setGeometry(x_pos+5*gap+6*x_size, y_pos, x_size, y_siz);
    killTrapezeButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(killTrapezeButton, &QPushButton::clicked, this, &MainWindow::killTrapeze);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);//Background
    QWidget::paintEvent(event);
}

void MainWindow::exitApp()
{
    this->close();//close this window
}

void MainWindow::backToMenu()//go back to main menu
{
    MenuPage *menPage = new MenuPage();
    menPage->show();//display menu page
    this->close();//close this page
}

void MainWindow::respSquare()//your choise to resp object is.. square
{
    QGraphicsRectItem *square = new QGraphicsRectItem(0, 0, 50, 50);//construct object square(size is 50x50)
    square->setBrush(Qt::blue);//set brush - colour of the bojbect
    square->setPos(600, 350);//resp object on this pos
    scene->addItem(square);//add item to scene
    squares.append(square);//add item to list(list with square objects)
}

void MainWindow::respRectangle()//your choise to resp object is.. rectangle
{
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 0, 80, 40);//construct object rectangle(size is 80x40)
    rectangle->setBrush(Qt::red);//set brush - colour of the object
    rectangle->setPos(600, 350);//resp object on this pos
    scene->addItem(rectangle);//add item to scene
    rectangles.append(rectangle);//add item to list(list with rectangle objects)
}

void MainWindow::respCircle()//your choise to resp object is.. circle
{
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(0, 0, 40, 40);//construct object circle(size is 40x40 on 2*radius x and y)
    circle->setBrush(Qt::yellow);//set brush - colour of the object
    circle->setPos(600, 350);//resp object on this pos
    scene->addItem(circle);//add item to scene
    circles.append(circle);//add item to list(list with circle objects)
}

void MainWindow::respTriangle()//your choise to resp object is.. triangle
{
    QPolygonF triangle;//create triangle based on polygon(lines connecting)
    triangle << QPointF(0, 50) << QPointF(25, 0) << QPointF(50, 50);//create 3 points(and later connect them to make triangle shape)
    QGraphicsPolygonItem *triangleItem = new QGraphicsPolygonItem(triangle);//create polygon(final shape is triangle)
    triangleItem->setBrush(Qt::black);//colour is black
    triangleItem->setPos(600,350);
    scene->addItem(triangleItem);//add item to scene
    triangles.append(triangleItem);//add item to list(list with triangle objects)
}

void MainWindow::respTrapeze()//your choise to resp object is.. trapeze
{
    QPolygonF trapeze;//create trapeze based on polygon(lines connecting)
    trapeze << QPointF(10, 0) << QPointF(40, 0) << QPointF(50, 30) << QPointF(0, 30);//create 4 points to shape trapeze
    QGraphicsPolygonItem *trapezeItem = new QGraphicsPolygonItem(trapeze);//create polygon(final shape is trapeze_
    trapezeItem->setBrush(Qt::magenta);//colour is magenta
    trapezeItem->setPos(600,350);
    scene->addItem(trapezeItem);//add item to scene
    trapezes.append(trapezeItem);//add item to list(list with trapeze objects)
}

void MainWindow::killSquare()//delete all of this type object
{
    for (auto square : squares) {//iterate throw vector
        scene->removeItem(square);//remove them from scene
        delete square;//delete object
    }
    squares.clear();//if iterated throw vector, clear
}

void MainWindow::killRectangle()//delete all of this type object
{
    for (auto rect : rectangles) {
        scene->removeItem(rect);
        delete rect;
    }
    rectangles.clear();
}

void MainWindow::killCircle()//delete all of this type object
{
    for (auto circle : circles) {
        scene->removeItem(circle);
        delete circle;
    }
    circles.clear();
}

void MainWindow::killTriangle()//delete all of this type object
{
    for (auto triangle : triangles) {
        scene->removeItem(triangle);
        delete triangle;
    }
    triangles.clear();
}

void MainWindow::killTrapeze()//delete all of this type object
{
    for (auto trapeze : trapezes) {
        scene->removeItem(trapeze);
        delete trapeze;
    }
    trapezes.clear();
}
