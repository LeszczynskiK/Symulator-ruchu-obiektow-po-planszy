#include "mainwindow.h"

//template for rect objects
#include "ShapeCreator.h"//template of class for figure objects
#include "ShapeRemover.h"//template for figure deleting


//template for object based on points connection
#include "ShapeCreatorPoints.h"//For triangles, trapezoids
#include "ShapeRemoverPoints.h"//For triangles, trapezoids

//vectors to store object
vector<thread> shapeThreads;//all objects
mutex shapeMutex;
vector<unique_ptr<PhysicalRectItem>> squares;//square object etc....
vector<unique_ptr<PhysicalRectItem>> rectangles;
vector<unique_ptr<PhysicalEllipseItem>> circles;
vector<unique_ptr<PhysicalPolygonItem>> triangles;
vector<unique_ptr<PhysicalPolygonItem>> trapezes;
vector<unique_ptr<ThreadedWindPoint>> windPoints;

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

    collisionHandler = new CollisionHandler(scene);//initialise collision handling object(to check collisoon between figures...)

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

    //type in mass input
    massInput = new QLineEdit(this);
    massInput->setGeometry(x_pos+1*gap+1*x_size, y_pos+y_siz+gap, 0.95*x_size, y_siz);
    massInput->setPlaceholderText("Mass[kg]");//show this, if clicked then make it empty and enable to type in value
    massInput->setFont(font);
    massInput->show();

    //type in friction input
    frictionInput = new QLineEdit(this);
    frictionInput->setGeometry(x_pos+1*gap+1*x_size, y_pos, 0.95*x_size, y_siz);
    frictionInput->setPlaceholderText("Friction");//show this, if clicked then make it empty and enable to type in value
    frictionInput->setFont(font);
    frictionInput->show();

    //type in widnd radius
    windRadiusInput = new QLineEdit(this);
    windRadiusInput->setGeometry(x_pos+7*gap+8*x_size, y_pos, x_size*0.8, y_siz);
    windRadiusInput->setPlaceholderText("Radius");
    windRadiusInput->setFont(font);
    windRadiusInput->show();

    //type in max force
    maxForceInput = new QLineEdit(this);
    maxForceInput->setGeometry(x_pos+7*gap+8*x_size, y_pos+y_siz+gap, x_size*0.8, y_siz);
    maxForceInput->setPlaceholderText("Max force");
    maxForceInput->setFont(font);
    maxForceInput->show();

    QFont infoFont;//define font
    infoFont.setPointSize(12);//font size for etiquette information


    int info_y_pos = saveAreaFrame->y() + saveAreaFrame->height() + 10;//10 px under save area frame
    int info_gap=250;//space between labels
    int info_start=355;
    int info_x_size=225;
    //range for mass QLineEdit
    QLabel* massInfoLabel = new QLabel("Mass range: 0.00001 - 100000,", this);
    massInfoLabel->setFont(infoFont);
    massInfoLabel->setGeometry(info_start+0*info_gap, info_y_pos, info_x_size, 20);
    massInfoLabel->setStyleSheet("color: black; background-color: rgba(255, 255, 255, 150);");

    //range for friction QLineEdit
    QLabel* frictionInfoLabel = new QLabel("Friction range: 0 - 1,", this);
    frictionInfoLabel->setFont(infoFont);
    frictionInfoLabel->setGeometry(info_start+1*info_gap, info_y_pos, info_x_size, 20);
    frictionInfoLabel->setStyleSheet("color: black; background-color: rgba(255, 255, 255, 150);");

    //range for WindRadius
    QLabel* windRadiusInfoLabel = new QLabel("Wind radius range: 50 - 1250,", this);
    windRadiusInfoLabel->setFont(infoFont);
    windRadiusInfoLabel->setGeometry(info_start+2*info_gap, info_y_pos, info_x_size, 20);
    windRadiusInfoLabel->setStyleSheet("color: black; background-color: rgba(255, 255, 255, 150);");

    //range for maxForce
    QLabel* maxForceInfoLabel = new QLabel("Wind force range: 1 - 100", this);
    maxForceInfoLabel->setFont(infoFont);
    maxForceInfoLabel->setGeometry(info_start+3*info_gap, info_y_pos, info_x_size, 20);
    maxForceInfoLabel->setStyleSheet("color: black; background-color: rgba(255, 255, 255, 150);");
}

MainWindow::~MainWindow() {
    delete simulationTimer;
    delete collisionHandler;//free memory..
    delete windRadiusInput;
    delete maxForceInput;
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

void MainWindow::setPhysicalProperties(QGraphicsItem* item) {//set typed in values to object you choosed to create by mouse click and object type choose
    if (auto* obj = dynamic_cast<PhysicalObject*>(item)) {//put item type toPhysicalObject(change from QgraphicsItem type)
        bool ok;//bool variable to mark if massInput throw to float suceed
        float mass = massInput->text().toFloat(&ok);//get mass from QLineEdit
        if (ok) {//if throw to fload suceed

            //limit input values
            mass = max(0.00001f, min(100000.0f, mass));
            obj->setMass(mass);
        } else {
            qDebug() << "Invalid mass input, defaulting to 1.0";
            obj->setMass(1.0f);//set this value is your type is over/below the limit
        }


        //the same situation as above..
        float friction = frictionInput->text().toFloat(&ok);
        if (ok) {

            //limit input values
            friction = max(0.0f, min(1.0f, friction));
            obj->setFriction(friction);
        } else {
            qDebug() << "Invalid friction input, defaulting to 0.5";
            obj->setFriction(0.5f);//set this value is your type is over/below the limit
        }
    }
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

void MainWindow::respWindPoint(int x, int y) {
    //default values - use if out of limits typed
    float windRadius = 400.0f;
    float maxForce = 10.0f;
    bool ok;

    //load from QLineEdits and limit them(WindRadius)
    float inputWindRadius = windRadiusInput->text().toFloat(&ok);
    if (ok) {
        windRadius = max(50.0f, min(1250.0f, inputWindRadius));
    } else {
        qDebug() << "Invalid windRadius input, defaulting to 400";
    }

     //load from QLineEdits and limit them(MaxForce)
    float inputMaxForce = maxForceInput->text().toFloat(&ok);
    if (ok) {
        maxForce = max(1.0f, min(100.0f, inputMaxForce));
    } else {
        qDebug() << "Invalid maxForce input, defaulting to 10";
    }

    //spawn wind points with the arguments and label
    spawnWindPoint(scene, x, y, 30, windRadius, maxForce, windPoints);

    scene->update();//update view
    windPointCondition = !windPointCondition;

    //clear qLineEdit fields
    windRadiusInput->clear();
    maxForceInput->clear();
}

void MainWindow::killWindPoints() {
    for (auto& windPoint : windPoints) {
        //remove and delete the label if it exists
        if (QGraphicsTextItem* label = windPoint->getLabel()) {
            scene->removeItem(label);//remove the label from the scene
            delete label;//free memroy
            windPoint->setLabel(nullptr);//set pointer as nullptr
        }
        //remove the wind point itself
        scene->removeItem(windPoint.get());
    }
    windPoints.clear();//Clear the vector of wind points
}

void MainWindow::updateSimulation() {

    //helper lambda to move items based on their dx, dy from labels(becouse it is actualised by wind and objects collision)
    auto moveItems = [&](auto& items) {
        for (auto& item : items)//loop through all items in the vector
        {
            PhysicalObject* obj = dynamic_cast<PhysicalObject*>(item.get());//cast to PhysicalObject
            if (obj && obj->getLabel())//if its a valid object with a label
            {
                float dx = 0.0f, dy = 0.0f;//preinitialise
                QString text = obj->getLabel()->toPlainText();//get the label text
                QStringList lines = text.split("\n");//split into lines(to separate valeus) - \n is separation
                dx = lines[0].mid(4).toFloat();//extract dx(mid(4) means skip 4 chars
                dy = lines[1].mid(4).toFloat();//extract dy
                item->moveBy(dx, dy);//move the item by its current speed
            }
        }
    };

    //move all items based on their dx and dy(dx and dy is based on wind and collison impact)
    moveItems(squares);//move all squares
    moveItems(rectangles);//move all rectangles
    moveItems(circles);//move all circles
    moveItems(triangles);//move all triangles
    moveItems(trapezes);//move all trapezes

    //Define a generic lambda function to remove objects that are outside the black frame boundaries
    //This function works with any container of unique_ptr<QGraphicsItem> (squares, circles, etc.)
    auto removeOutOfBounds = [&](auto& items) {

        //Use remove_if and erase to remove objects that are out of bounds
        //The lambda checks each item in the vector and returns true if the item should be removed
        items.erase(remove_if(items.begin(), items.end(), [&](const auto& item) {
                        if (!item) return false;//if pointer  not exist, return false

                        //Calculate the center position of the item by adding half of its bounding rectangle
                        //to its top-left position (pos()). This ensures we check the item's center, not its top-left corner
                        QPointF itemPos = item->pos() + QPointF(item->boundingRect().width() / 2, item->boundingRect().height() / 2);

                        //Check if the item's center is outside the safe area (black frame boundaries)
                        // The boundaries are defined by frame_siz (frame width) and save_gap (margin from frame)
                        bool outOfBounds = (itemPos.x() < frame_siz + save_gap || itemPos.x() > x_frame - frame_siz - save_gap ||
                                            itemPos.y() < frame_siz + save_gap || itemPos.y() > y_frame - frame_siz - save_gap);

                        //If the item is out of bounds, remove it from the scene and mark it for erasure
                        if (outOfBounds) {
                            //Get PhysicalObject to have access to etiquette
                            if (auto* obj = dynamic_cast<PhysicalObject*>(item.get())) {
                                if (QGraphicsTextItem* label = obj->getLabel()) {
                                    scene->removeItem(label);//delete from scene and free memory
                                    delete label;
                                    obj->setLabel(nullptr);//set as nullptr to avoid memory leakage
                                }
                            }
                            scene->removeItem(item.get());
                            delete massInputs[item.get()];
                            delete frictionInputs[item.get()];
                            massInputs.erase(item.get());
                            frictionInputs.erase(item.get());
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

    //lock_guard<mutex> lock(shapeMutex);//to keep away from mistake(windpoints are not being modified in other thread)

    if (!windPoints.empty()) {//cann only once - becouse we have resultant sum od x and y strenght having impact on objects (impact from wintPoints)
        windPoints[0]->applyWindForce(squares, rectangles, circles, triangles, trapezes, windPoints);
    }

    collisionHandler->checkCollisions(squares, rectangles, circles, triangles, trapezes);//call method to check collisions between objects...

    scene->update();
}

void MainWindow::respSquare(int x,int y)//your choise to resp object is.. square
{
   spawnWithProperties(scene, massInput, frictionInput, spawnShape<PhysicalRectItem>, squares, 50, 50, QColor(Qt::blue), x, y);//<type of figure>scene, size_x,size_y,colour,x y is from mouse event, type of object

    squareCondition = !squareCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false

    if (!squares.empty()) {
        qDebug() << "Square pos():" << squares.back()->pos() << "BoundingRect:" << squares.back()->boundingRect();
        massInput->clear();//if resped, clear values from QLineEdit...
        frictionInput->clear();
    }
}

void MainWindow::respRectangle(int x,int y)//your choise to resp object is.. rectangle
{
    spawnWithProperties(scene, massInput, frictionInput, spawnShape<PhysicalRectItem>, rectangles, 100, 50, QColor(Qt::green), x, y);

    rectangleCondition=!rectangleCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false

    if (!rectangles.empty()) {
        qDebug() << "Rectangle pos():" << rectangles.back()->pos() << "BoundingRect:" << rectangles.back()->boundingRect();
        massInput->clear();//if resped, clear values from QLineEdit...
        frictionInput->clear();
    }
}

void MainWindow::respCircle(int x,int y)//your choise to resp object is.. circle
{
    spawnWithProperties(scene, massInput, frictionInput, spawnShape<PhysicalEllipseItem>, circles, 50, 50, QColor(Qt::red), x, y);

    circleConditon=!circleConditon;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false

    if (!circles.empty()) {
        qDebug() << "Circle pos():" << circles.back()->pos() << "BoundingRect:" << circles.back()->boundingRect();
        massInput->clear();//if resped, clear values from QLineEdit...
        frictionInput->clear();
    }
}

void MainWindow::respTriangle(int x,int y)//your choise to resp object is.. triangle
{
    QPolygonF triangle;
    triangle << QPointF(0, 50) << QPointF(25, 0) << QPointF(50, 50);

    spawnPolygonWithProperties(scene, massInput, frictionInput, spawnPolygonShape<PhysicalPolygonItem>, triangles, triangle, QColor(Qt::white), x, y);

    triangleCondition=!triangleCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false

    if (!triangles.empty()) {
        qDebug() << "Triangle pos():" << triangles.back()->pos() << "BoundingRect:" << triangles.back()->boundingRect();
        massInput->clear();//if resped, clear values from QLineEdit...
        frictionInput->clear();
    }
}

void MainWindow::respTrapeze(int x,int y)//your choise to resp object is.. trapeze
{
    QPolygonF trapeze;
    trapeze << QPointF(10, 0) << QPointF(40, 0) << QPointF(50, 30) << QPointF(0, 30);

    spawnPolygonWithProperties(scene, massInput, frictionInput, spawnPolygonShape<PhysicalPolygonItem>, trapezes, trapeze, QColor(Qt::magenta), x, y);

    trapezeCondition=!trapezeCondition;//this resp funciton is called only one(when object type selected and mouse pressed),
    //it will be possible to resp only if bool is true, i want to have object type choosen for 1 click so i need
    //to change from true to false every function calling(funciton possible to call only if true, so each time it will
    //be transformed to false

    if (!trapezes.empty()) {
        qDebug() << "Trapeze pos():" << trapezes.back()->pos() << "BoundingRect:" << trapezes.back()->boundingRect();
        massInput->clear();//if resped, clear values from QLineEdit...
        frictionInput->clear();
    }
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
