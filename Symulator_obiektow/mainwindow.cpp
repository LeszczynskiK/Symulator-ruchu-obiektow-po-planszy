#include "mainwindow.h"

//template for rect objects
#include "ShapeCreator.h"//template of class for figure objects
#include "ShapeRemover.h"//template for figure deleting


//template for object based on points connection
#include "ShapeCreatorPoints.h"//For triangles, trapezoids
#include "ShapeRemoverPoints.h"//For triangles, trapezoids

//vectors to store object
vector<thread> shapeThreads;//all objects
vector<unique_ptr<QGraphicsRectItem>> squares;//square object etc....
vector<unique_ptr<QGraphicsRectItem>> rectangles;
vector<unique_ptr<QGraphicsEllipseItem>> circles;
vector<unique_ptr<QGraphicsPolygonItem>> triangles;
vector<unique_ptr<QGraphicsPolygonItem>> trapezes;
vector<unique_ptr<WindPoint>> windPoints;

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
    frame->setGeometry(0, 0, x_frame, y_frame);
    frame->setStyleSheet("background-color: transparent; border: " + QString::number(frame_siz) + "px solid black;");

    //Save area frame- here resp by mouse click can be done(inside this area)
    int save_area_size_x=1366-2*save_gap-2*frame_siz;
    int save_area_size_y=663-2*save_gap-2*frame_siz;
    saveAreaFrame = new QLabel(this);
    saveAreaFrame->setGeometry(save_gap+frame_siz, save_gap+frame_siz, save_area_size_x, save_area_size_y);
    saveAreaFrame->setStyleSheet("background-color: transparent; border:1px dotted black;");//1px frame width

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

    respWindPointButton = new QPushButton("Wind Point", this);
    respWindPointButton->setFont(font);
    respWindPointButton->setGeometry(x_pos + 6 * gap + 7 * x_size, y_pos + y_siz + gap, x_size, y_siz);
    respWindPointButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");
    connect(respWindPointButton, &QPushButton::clicked, this, &MainWindow::changeWindPointCondition);

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

    killWindPointButton = new QPushButton("Delete all", this);
    killWindPointButton->setFont(font);
    killWindPointButton->setGeometry(x_pos + 6 * gap + 7 * x_size, y_pos, x_size, y_siz);
    killWindPointButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");
    connect(killWindPointButton, &QPushButton::clicked, this, &MainWindow::killWindPoints);

    //timer to refresh view by each 33ms (its about 30fps)
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    simulationTimer->start(33);//1sec / 30 is about 33 -> 30fps
}

MainWindow::~MainWindow() {
    delete simulationTimer;
}

void MainWindow::paintEvent(QPaintEvent *event) {//Paint event to draw the background
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);//Background
    QWidget::paintEvent(event);//Call base class paint event
}

void MainWindow::exitApp()
{
    joinThreads();//Wait for all threads to finish before closing
    this->close();//close this window
}

void MainWindow::backToMenu()//go back to main menu
{
    MenuPage *menPage = new MenuPage();
    menPage->show();//display menu page
    this->close();//close this page
}

bool MainWindow::isWithinFrame(int x, int y) {

    //counting "save" coordinates(area) to make possible object resp by mouse click
    const int frameLeft = frame_siz + save_gap;//Left boundary
    const int frameRight = x_frame -frame_siz -save_gap;//Right boundary
    const int frameTop = frame_siz + save_gap;//Top boundary
    const int frameBottom = y_frame - frame_siz - save_gap;//Bottom boundary

    if(x >= frameLeft && x <= frameRight && y >= frameTop && y <= frameBottom)//mouse coursor inside "save" area?
    {
        return 1;//return 1 if yes
    }
    return 0;//return bool as 0 if not
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    // Get mouse position
    int x = event->position().x();
    int y = event->position().y();

    //check if the click is within the valid frame area
    if (!isWithinFrame(x, y)) {//if is not inside save area
        QMessageBox::warning(this, "Invalid Position", "Objects can be spawned only inside save area(-30px from frame)!!!");
        return;//Exit the function if outside the save area
    }
    else//coursos is inside "save" area
    {
        // Determine which object to spawn based on the button clicked
        if (squareCondition) {
            respSquare(x, y);//Create square
        } else if (rectangleCondition) {
            respRectangle(x, y);//Create rectangle
        } else if (circleConditon) {
            respCircle(x, y);//Create circle
        } else if (triangleCondition) {
            respTriangle(x, y);//Create triangle
        } else if (trapezeCondition) {
            respTrapeze(x, y);//create trapeze
        } else if (windPointCondition) {
            respWindPoint(x, y);//create wind point
        }
    }
}

bool MainWindow::changeSquareCondition()//is button clicked?
{
    squareCondition = !squareCondition;//change condition on opposite one
    if (squareCondition) {//and if this mode is selected, disable other modes
        rectangleCondition = false;
        circleConditon = false;
        triangleCondition = false;
        trapezeCondition = false;
        windPointCondition=false;
    }
    return squareCondition;
}

bool MainWindow::changeRectangleCondition()//is button clicked?
{
    rectangleCondition=!rectangleCondition;//change condition on opposite one
    if (rectangleCondition) {//and if this mode is selected, disable other modes
        squareCondition = false;
        circleConditon = false;
        triangleCondition = false;
        trapezeCondition = false;
    }
    return rectangleCondition;
}

bool MainWindow::changeCircleConditon()//is button clicked?
{
    circleConditon=!circleConditon;//change condition on opposite one
    if (circleConditon) {//and if this mode is selected, disable other modes
        squareCondition = false;
        rectangleCondition = false;
        triangleCondition = false;
        trapezeCondition = false;
        windPointCondition=false;
    }
    return circleConditon;
}

bool MainWindow::changeTriangleCondition()//is button clicked?
{
    triangleCondition=!triangleCondition;//change condition on opposite one
    if (triangleCondition) {//and if this mode is selected, disable other modes
        squareCondition = false;
        rectangleCondition = false;
        circleConditon = false;
        trapezeCondition = false;
        windPointCondition=false;
    }
    return triangleCondition;
}

bool MainWindow::changeTrapezeCondition()//is button clicked?
{
    trapezeCondition=!trapezeCondition;//change condition on opposite one
    if (trapezeCondition) {//and if this mode is selected, disable other modes
        squareCondition = false;
        rectangleCondition = false;
        circleConditon = false;
        triangleCondition = false;
        windPointCondition=false;
    }
    return trapezeCondition;
}

bool MainWindow::changeWindPointCondition() {//is button clicked?
    windPointCondition = !windPointCondition;
    qDebug() << "WindPoint condition changed to:" << windPointCondition;
    if (windPointCondition) {
        squareCondition = false;
        rectangleCondition = false;
        circleConditon = false;
        triangleCondition = false;
        trapezeCondition = false;
    }
    return windPointCondition;
}

void MainWindow::respWindPoint(int x, int y) {//
    windPoints.push_back(make_unique<WindPoint>(x, y, 30, scene));
    scene->update();//refresh scene view
    windPointCondition = !windPointCondition;
}

void MainWindow::killWindPoints() {
    for (auto& windPoint : windPoints) {
        scene->removeItem(windPoint.get());
    }
    windPoints.clear();
}

void MainWindow::updateSimulation() {

    auto removeOutOfBounds = [&](auto& items) {
        items.erase(remove_if(items.begin(), items.end(), [&](const auto& item) {
                        if (!item) return false;//if pointer  not exist, return false
                        QPointF itemPos = item->pos() + QPointF(item->boundingRect().width() / 2, item->boundingRect().height() / 2);
                        bool outOfBounds = (itemPos.x() < frame_siz + save_gap || itemPos.x() > x_frame - frame_siz - save_gap ||
                                            itemPos.y() < frame_siz + save_gap || itemPos.y() > y_frame - frame_siz - save_gap);
                        if (outOfBounds) {
                            scene->removeItem(item.get());
                            return true;//delete object
                        }
                        return false;
                    }), items.end());
    };

    //delete objects from vectors(if object is passed to removeOutOfBounds vectr)
    removeOutOfBounds(squares);//if passed to this vector(square type) then delete here
    removeOutOfBounds(rectangles);//same as aboxe...
    removeOutOfBounds(circles);
    removeOutOfBounds(triangles);
    removeOutOfBounds(trapezes);

    for (auto& windPoint : windPoints) {
        windPoint->applyWindForce(squares, rectangles, circles, triangles, trapezes);
    }
    scene->update();
}

void MainWindow::respSquare(int x,int y)//your choise to resp object is.. square
{
    spawnShape<QGraphicsRectItem>(scene, 50, 50, QColor(Qt::blue), x, y, squares);//<type of figure>scene, size_x,size_y,colour,x y is from mouse event, type of object

    squareCondition = !squareCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respRectangle(int x,int y)//your choise to resp object is.. rectangle
{
    spawnShape<QGraphicsRectItem>(scene, 100, 50, QColor(Qt::green), x, y, rectangles);//x,y Position from mouse event

    rectangleCondition=!rectangleCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respCircle(int x,int y)//your choise to resp object is.. circle
{
    spawnShape<QGraphicsEllipseItem>(scene, 50, 50, QColor(Qt::red), x, y, circles);//x,y Position from mouse event

    circleConditon=!circleConditon;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respTriangle(int x,int y)//your choise to resp object is.. triangle
{
    QPolygonF triangle;
    triangle << QPointF(0, 50) << QPointF(25, 0) << QPointF(50, 50);

    spawnPolygonShape<QGraphicsPolygonItem>(scene, triangle, QColor(Qt::black), x, y, triangles);

    triangleCondition=!triangleCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::respTrapeze(int x,int y)//your choise to resp object is.. trapeze
{
    QPolygonF trapeze;
    trapeze << QPointF(10, 0) << QPointF(40, 0) << QPointF(50, 30) << QPointF(0, 30);

    spawnPolygonShape<QGraphicsPolygonItem>(scene, trapeze, QColor(Qt::magenta), x, y, trapezes);

    trapezeCondition=!trapezeCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false
}

void MainWindow::killSquare()//delete all of this type object
{
    removeShapes(scene, squares);//delete only squares
}

void MainWindow::killRectangle()//delete all of this type object
{
    removeShapes(scene, rectangles);
}

void MainWindow::killCircle()//delete all of this type object
{
    removeShapes(scene, circles);
}

void MainWindow::killTriangle()//delete all of this type object
{
    removePolygonShapes(scene, triangles);//delete all of this type
}

void MainWindow::killTrapeze()//delete all of this type object
{
    removePolygonShapes(scene, trapezes);//delete all of this type
}
