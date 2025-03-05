#ifndef SHAPECREATOR_H
#define SHAPECREATOR_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMetaObject>
#include <QCoreApplication>
#include <QBrush>
#include <QColor>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>

#include "threadedwindpoint.h"//add windpoints to thread bace objects too(library to allow acces)
#include "physicalobject.h"//physical values definition

using namespace std;

extern vector<thread> shapeThreads;//to collect all type of object(no matter what type)
extern vector<unique_ptr<QGraphicsRectItem>> squares;//vector to collect square objects
extern vector<unique_ptr<QGraphicsRectItem>> rectangles;//vector to collect rectangle objects
extern vector<unique_ptr<QGraphicsEllipseItem>> circles;//vector to collect circle objects
extern vector<unique_ptr<ThreadedWindPoint>> windPoints;//vector to collect wintpoints objects

mutex shapeMutex;//mutex to ensure thread-safe access to shared resources for example vectors


//create windpoint (each in new thread)
void createThreadedWindPointThread(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    auto localWindPoint = make_unique<ThreadedWindPoint>(posX, posY, radius, scene);
    ThreadedWindPoint* rawWindPoint = localWindPoint.get();

    QMetaObject::invokeMethod(scene, [scene, rawWindPoint]() {
        scene->addItem(rawWindPoint);
    }, Qt::QueuedConnection);

    {
        lock_guard<mutex> lock(shapeMutex);
        targetVector.push_back(move(localWindPoint));
    }
}

//run thread for each object of this type
void spawnThreadedWindPoint(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    shapeThreads.emplace_back(createThreadedWindPointThread, scene, posX, posY, radius, ref(targetVector));
}

//Template function to create a shape in a separate thread
//T: The type of shape (likeQGraphicsRectItem or QGraphicsEllipseItem)
//arguments are; scene, dimensions, color, and position, and specify the target vector to store it
template<typename T>
void createShapeThread(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY, vector<unique_ptr<T>>& targetVector) {
    auto localShape = std::make_unique<T>(0, 0, width, height);//Create a unique pointer to a new shape with specified width and height
    localShape->setBrush(color);
    localShape->setPos(posX, posY);

    //Use a raw pointer for adding to scene, then move ownership to vector
    T* rawShape = localShape.get();

    //Queue the addition of the polygon to the scene in the main thread (Qt GUI operations must occur in the main thread)
    QMetaObject::invokeMethod(scene, [scene, rawShape]() {
        scene->addItem(rawShape);//Add the shape to scene
    }, Qt::QueuedConnection);

    //Move ownership to the vector after queuing the addition
    {
        lock_guard<mutex> lock(shapeMutex);//lock the mutex to prevent concurrent access
        targetVector.push_back(std::move(localShape));//put to target vector of type(circle etc.)
    }
}

//Template function to spawn a shape by launching a thread
//T: The type of shape to spawn
//Creates a thread that calls createShapeThread with the given parameters
template<typename T>
void spawnShape(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY, vector<unique_ptr<T>>& targetVector) {

    //Add a new thread to the shapeThreads vector, passing the createShapeThread function and its arguments
    shapeThreads.emplace_back(createShapeThread<T>, scene, width, height, color, posX, posY, ref(targetVector));//ref - pass the reference to exact vector we cant
}

//Function to join all active threads in shapeThreads
void joinThreads() {
    for (auto& t : shapeThreads) {//iterate throw vector with threads
        if (t.joinable()) {
            t.join();
        }
    }
}

//this is class which will specity all of the objects which base on QRectItem
class PhysicalRectItem : public QGraphicsRectItem, public PhysicalObject {
public:
    PhysicalRectItem(float x, float y, float width, float height)
        : QGraphicsRectItem(0, 0, width, height) {
        setPos(x, y);
    }
    QGraphicsItem* getGraphicsItem() override { return this; }
};


//this is class which will specity all of the objects which base on QElipseObject
class PhysicalEllipseItem : public QGraphicsEllipseItem, public PhysicalObject {
public:
    PhysicalEllipseItem(float x, float y, float width, float height)
        : QGraphicsEllipseItem(0, 0, width, height) {
        setPos(x, y);
    }
    QGraphicsItem* getGraphicsItem() override { return this; }
};

#endif // SHAPECREATOR_H
