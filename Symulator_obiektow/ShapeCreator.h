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
using namespace std;

extern vector<thread> shapeThreads;//to collect all type of object(no matter what type)
extern vector<unique_ptr<QGraphicsRectItem>> squares;//vector to collect square objects
extern vector<unique_ptr<QGraphicsRectItem>> rectangles;//vector to collect rectangle objects
extern vector<unique_ptr<QGraphicsEllipseItem>> circles;//vector to collect circle objects

mutex shapeMutex;//mutex to ensure thread-safe access to shared resources for example vectors


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

#endif // SHAPECREATOR_H
