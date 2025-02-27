#ifndef SHAPECREATOR_H
#define SHAPECREATOR_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
<<<<<<< HEAD
#include <QMetaObject>//to make possible working on threads(main funcitonality is on main thread, rest on added ones)
=======
#include <QMetaObject>
>>>>>>> testy
#include <QCoreApplication>
#include <QBrush>
#include <QColor>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
using namespace std;

<<<<<<< HEAD
vector<thread> shapeThreads;//vector to keep object of type thread(collect object created - each in separated thread)
vector<unique_ptr<QGraphicsItem>> shapeObjects;//Keep created object(of QGraphicsItem type) in vector(by mechanism of smart pointers)
mutex shapeMutex;//mutex to block acces to protected fields of threads(block acces of all threads in one moment)


//function only to create shape(not return or spawn)
template<typename T>//T will be type of shape(RectItem, EllipseItem) - can use only for figure which do not base on hand written points
void createShapeThread(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY) {

    //Create new figure object(arguments are the base of object - it will personalize different type)
    unique_ptr<T> shape = std::make_unique<T>(0, 0, width, height);//object is based on smark pointer(2last arguments is size of figure)
    shape->setBrush(color);//add colour of figure
    shape->setPos(posX, posY);//set position of figure on scene

    QMetaObject::invokeMethod(scene, [scene, shape = move(shape)]() {
        scene->addItem(shape.get());//Add shape to scene in the main thread
        shapeObjects.push_back(move(shape));//Store the shape object
    }, Qt::QueuedConnection);//Qt::QueuedConnection ensures this is executed in the main thread
}


//function to spawn created object in createSkapeThread function
template<typename T>
void spawnShape(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY) {
    shapeThreads.emplace_back(createShapeThread<T>, scene, width, height, color, posX, posY);
    //shapeThreads is vector of thread type.
    //emplace_back - create new object of thread type indirectly in this vector
    //createShapeThread<T> is function which is turned on in new thread
    //T is type which will be called with exact type(Circle etc.)
    //rest of arguments will be given to this function(to create object with arguments on thread)

    //emplace_back is better becouse create object indirectly in vector(if we used push_back etc
    // we should firstly create object and then is should be moved to bector
    //with emplace_back se skip this part - we create object indirectly in vector
=======
extern vector<thread> shapeThreads;//to collect all type of object(no matter what type)
extern vector<unique_ptr<QGraphicsRectItem>> squares;//vector to collect square objects
extern vector<unique_ptr<QGraphicsRectItem>> rectangles;//vector to collect rectangle objects
extern vector<unique_ptr<QGraphicsEllipseItem>> circles;//vector to collect circle objects
extern vector<unique_ptr<QGraphicsPolygonItem>> triangles;//vector to collect triangle objects
extern vector<unique_ptr<QGraphicsPolygonItem>> trapezes;//vector to collect trapeze objects

mutex shapeMutex;

template<typename T>
void createShapeThread(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY, vector<unique_ptr<T>>& targetVector) {
    auto localShape = std::make_unique<T>(0, 0, width, height);
    localShape->setBrush(color);
    localShape->setPos(posX, posY);

    //Use a raw pointer for adding to scene, then move ownership to vector
    T* rawShape = localShape.get();

    QMetaObject::invokeMethod(scene, [scene, rawShape]() {
        scene->addItem(rawShape);//Add to scene in main thread
    }, Qt::QueuedConnection);

    //Move ownership to the vector after queuing the addition
    {
        lock_guard<mutex> lock(shapeMutex);
        targetVector.push_back(std::move(localShape));//put to given vector of type(circle etc.)
    }
}

template<typename T>
void spawnShape(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY, vector<unique_ptr<T>>& targetVector) {
    shapeThreads.emplace_back(createShapeThread<T>, scene, width, height, color, posX, posY, ref(targetVector));//ref - pass the reference to exact vector we cant
}

void joinThreads() {
    for (auto& t : shapeThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
>>>>>>> testy
}

#endif // SHAPECREATOR_H
