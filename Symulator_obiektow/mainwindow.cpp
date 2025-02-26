#include "mainwindow.h"
#include "ShapeCreator.h"//template of class for figure objects
#include "ShapeRemover.h"//template for figure deleting


MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Shakes physics simulation");

    const int x_win =1366;//x window size
    const int y_win=768;//y window size

    setFixedSize(x_win, y_win);//Set window size

    //Background image
    background = QPixmap("/home/krzysiek89/Desktop/QT_aplikacje/Symulator_ruchu_obiektow/Symulator_obiektow/AppPage.jpg").scaled(x_win, y_win);

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
    connect(respSquareButton, &QPushButton::clicked, this, &MainWindow::changeSquareCondition);

    respRectangleButton = new QPushButton("Rectangle", this);
    respRectangleButton->setFont(font);
    respRectangleButton->setGeometry(x_pos+2*gap+3*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respRectangleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respRectangleButton, &QPushButton::clicked, this, &MainWindow::changeRectangleCondition);

    respCircleButton = new QPushButton("Circle", this);
    respCircleButton->setFont(font);
    respCircleButton->setGeometry(x_pos+3*gap+4*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respCircleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respCircleButton, &QPushButton::clicked, this, &MainWindow::changeCircleConditon);

    respTriangleButton = new QPushButton("Triangle", this);
    respTriangleButton->setFont(font);
    respTriangleButton->setGeometry(x_pos+4*gap+5*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respTriangleButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respTriangleButton, &QPushButton::clicked, this, &MainWindow::changeTriangleCondition);

    respTrapezeButton = new QPushButton("Trapeze", this);
    respTrapezeButton->setFont(font);
    respTrapezeButton->setGeometry(x_pos+5*gap+6*x_size, y_pos+y_siz+gap, x_size, y_siz);
    respTrapezeButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(respTrapezeButton, &QPushButton::clicked, this, &MainWindow::changeTrapezeCondition);

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

void MainWindow::mousePressEvent(QMouseEvent *event) {
    // Get mouse position
    int x = event->position().x();
    int y = event->position().y();

    // Determine which object to spawn based on the button clicked
    if (squareCondition) {
        spawnShape<QGraphicsRectItem>(scene, 50, 50, Qt::blue, x, y);//Create square
    } else if (rectangleCondition) {
        spawnShape<QGraphicsRectItem>(scene, 100, 50, Qt::green, x, y);//Create rectangle
    } else if (circleConditon) {
        spawnShape<QGraphicsEllipseItem>(scene, 50, 50, Qt::red, x, y);//Create circle
    } else if (triangleCondition) {
        respTriangle(x, y);
    } else if (trapezeCondition) {
        respTrapeze(x, y);
    }
}

bool MainWindow::changeSquareCondition()//is button clicked?
{
    squareCondition = !squareCondition;//change condition on opposite one
    return squareCondition;
}

bool MainWindow::changeRectangleCondition()//is button clicked?
{
    rectangleCondition=!rectangleCondition;//change condition on opposite one
    return rectangleCondition;
}

bool MainWindow::changeCircleConditon()//is button clicked?
{
    circleConditon=!circleConditon;//change condition on opposite one
    return circleConditon;
}

bool MainWindow::changeTriangleCondition()//is button clicked?
{
    triangleCondition=!triangleCondition;//change condition on opposite one
    return triangleCondition;
}

bool MainWindow::changeTrapezeCondition()//is button clicked?
{
    trapezeCondition=!trapezeCondition;//change condition on opposite one
    return trapezeCondition;
}


void MainWindow::respSquare(int x,int y)//your choise to resp object is.. square
{
    shapeThreads.emplace_back(createShapeThread<QGraphicsRectItem>, scene, 50, 50, QColor(Qt::blue), x, y);//<type of figure>scene, size_x,size_y,colour,x y is from mouse event

    squareCondition = !squareCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respRectangle(int x,int y)//your choise to resp object is.. rectangle
{
    shapeThreads.emplace_back(createShapeThread<QGraphicsRectItem>, scene, 100, 50, QColor(Qt::green), x, y);//x,y Position from mouse event

    rectangleCondition=!rectangleCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respCircle(int x,int y)//your choise to resp object is.. circle
{
    shapeThreads.emplace_back(createShapeThread<QGraphicsEllipseItem>, scene, 50, 50, QColor(Qt::red), x, y);//x,y Position from mouse event

    circleConditon=!circleConditon;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respTriangle(int x,int y)//your choise to resp object is.. triangle
{
    QPolygonF triangle;//create triangle based on polygon(lines connecting)
    triangle << QPointF(0, 50) << QPointF(25, 0) << QPointF(50, 50);//create 3 points(and later connect them to make triangle shape)
    QGraphicsPolygonItem *triangleItem = new QGraphicsPolygonItem(triangle);//create polygon(final shape is triangle)
    triangleItem->setBrush(Qt::black);//colour is black
    triangleItem->setPos(x,y);//Position from mouse event
    scene->addItem(triangleItem);//add item to scene
    triangles.append(triangleItem);//add item to list(list with triangle objects)

    triangleCondition=!triangleCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respTrapeze(int x,int y)//your choise to resp object is.. trapeze
{
    QPolygonF trapeze;//create trapeze based on polygon(lines connecting)
    trapeze << QPointF(10, 0) << QPointF(40, 0) << QPointF(50, 30) << QPointF(0, 30);//create 4 points to shape trapeze
    QGraphicsPolygonItem *trapezeItem = new QGraphicsPolygonItem(trapeze);//create polygon(final shape is trapeze_
    trapezeItem->setBrush(Qt::magenta);//colour is magenta
    trapezeItem->setPos(x,y);//Position from mouse event
    scene->addItem(trapezeItem);//add item to scene
    trapezes.append(trapezeItem);//add item to list(list with trapeze objects)

    trapezeCondition=!trapezeCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::killSquare()//delete all of this type object
{
    removeObjects(scene, squares);//remove (name of scene, object type)
}

void MainWindow::killRectangle()//delete all of this type object
{
    removeObjects(scene, rectangles);//remove (name of scene, object type)
}

void MainWindow::killCircle()//delete all of this type object
{
    removeObjects(scene, circles);//remove (name of scene, object type)
}

void MainWindow::killTriangle()//delete all of this type object
{
    removeObjects(scene, triangles);//remove (name of scene, object type)
}

void MainWindow::killTrapeze()//delete all of this type object
{
    removeObjects(scene, trapezes);//remove (name of scene, object type)
}
