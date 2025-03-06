#ifndef SHAPECREATORPOINTS_H
#define SHAPECREATORPOINTS_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QMetaObject>
#include <QCoreApplication>
#include <QBrush>
#include <QColor>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include "physicalobject.h"//to add physical values to the objects

using namespace std;

//this is a class to specify all of the objects which base on polygon item(base on shapes made from points)
class PhysicalPolygonItem : public QGraphicsPolygonItem, public PhysicalObject {
public:
    PhysicalPolygonItem(const QPolygonF& polygon,float mass, float friction)
        : QGraphicsPolygonItem(polygon),PhysicalObject(mass, friction) {}
    QGraphicsItem* getGraphicsItem() override { return this; }
};

extern vector<thread> shapeThreads;//Shared thread pool
extern vector<unique_ptr<PhysicalPolygonItem>> triangles;//Vector for triangles
extern vector<unique_ptr<PhysicalPolygonItem>> trapezes;//Vector for trapezoids
extern mutex shapeMutex;//A shared mutex to ensure thread-safe access to shared resources

//Template function to create a polygon shape in a separate thread
//T: The type of shape (like QGraphicsPolygonItem)
//Parameters define the polygon's geometry (via QPolygonF), color, position, and target vector
template<typename T>
void createPolygonShapeThread(QGraphicsScene* scene, const QPolygonF& polygon, QColor color, qreal posX, qreal posY,float mass, float friction, vector<unique_ptr<T>>& targetVector) {
    auto localShape = std::make_unique<T>(polygon,mass, friction);//Create a unique pointer to a new polygon shape using the provided QPolygonF
    localShape->setBrush(color);
    localShape->setPos(posX, posY);

    T* rawShape = localShape.get();//Get a raw pointer to the shape for scene addition

    //Queue the addition of the polygon to the scene in the main thread (Qt GUI operations must occur in the main thread)
    QMetaObject::invokeMethod(scene, [scene, rawShape]() {
        scene->addItem(rawShape);
    }, Qt::QueuedConnection);

    //Safely transfer ownership of the polygon to the target vector
    {
        lock_guard<mutex> lock(shapeMutex);//Lock the mutex to prevent concurrent access to the target vector
        targetVector.push_back(std::move(localShape));//Move the unique pointer into the target vector (like triangles, trapezes)
    }
}

//Template function to spawn a polygon shape by launching a thread
//T: The type of shape to spawn (typically QGraphicsPolygonItem)
//Creates a thread that calls createPolygonShapeThread with given parameters
template<typename T>
void spawnPolygonShape(QGraphicsScene* scene, const QPolygonF& polygon, QColor color, qreal posX, qreal posY,float mass, float friction, vector<unique_ptr<T>>& targetVector) {

    // Add a new thread to the shapeThreads vector, passing the createPolygonShapeThread function and its arguments
    shapeThreads.emplace_back(createPolygonShapeThread<T>, scene, polygon, color, posX, posY,mass, friction, ref(targetVector));// ref(targetVector) ensures the vector is passed by reference to the thread
}

#endif // SHAPECREATORPOINTS_H
