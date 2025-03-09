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

#include "physicalobject.h"//physical values definition
#include "threadedwindpoint.h"

using namespace std;

extern vector<thread> shapeThreads;

//this is class which will specity all of the objects which base on QRectItem
class PhysicalRectItem : public QGraphicsRectItem, public PhysicalObject {
public:
    PhysicalRectItem(float x, float y, float width, float height,float mass, float friction)
        : QGraphicsRectItem(0, 0, width, height), PhysicalObject(mass, friction) {
        setPos(x, y);
    }

    //override a virtual function from PhysicalObject to return a pointer to the graphical item (itself).
    //this allows the PhysicalObject base class to interact with the QGraphicsItem interface.
    //returns a QGraphicsItem* pointing to this PhysicalRectItem instance.
    QGraphicsItem* getGraphicsItem() override { return this; }

    //count surface area of object
    float getSurfaceArea() const override { return rect().width() * rect().height(); }//width * height(for rectangle type)
};


//this is class which will specity all of the objects which base on QElipseObject
class PhysicalEllipseItem : public QGraphicsEllipseItem, public PhysicalObject {
public:
    PhysicalEllipseItem(float x, float y, float width, float height,float mass, float friction)
        : QGraphicsEllipseItem(0, 0, width, height),PhysicalObject(mass, friction) {
        setPos(x, y);
    }

    //override a virtual function from PhysicalObject to return a pointer to the graphical item (itself).
    //this allows the PhysicalObject base class to interact with the QGraphicsItem interface.
    //returns a QGraphicsItem* pointing to this PhysicalRectItem instance.
    QGraphicsItem* getGraphicsItem() override { return this; }

    //count surface area of object
    float getSurfaceArea() const override {
        float radiusX = rect().width() / 2.0f;//radius in x axis is half of width(f - float type)
        float radiusY = rect().height() / 2.0f;//radius in y axis is half of height
        return M_PI * radiusX * radiusY;//surface of elipse if pi*r1*r2 (pi*r(square) is r are equal)
    }
};

// extern vector<thread> shapeThreads;//to collect all type of object(no matter what type)
// extern vector<unique_ptr<PhysicalRectItem>> squares;//vector to collect square objects
// extern vector<unique_ptr<PhysicalRectItem>> rectangles;//vector to collect rectangle objects
// extern vector<unique_ptr<PhysicalEllipseItem>> circles;//vector to collect circle objects
// extern vector<unique_ptr<ThreadedWindPoint>> windPoints;//vector to collect wintpoints objects

extern mutex shapeMutex;//mutex to ensure thread-safe access to shared resources for example vectors


//create windpoint (each in new thread)
inline void createThreadedWindPointThread(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {

    //create a unique pointer to a new ThreadedWindPoint object with the specified position and radius
    //ThreadedWindPoint is a custom class that simulates wind effects in the scene
    auto localWindPoint = make_unique<ThreadedWindPoint>(posX, posY, radius, scene);

    //get a raw pointer to the wind point for adding it to the scene (needed because unique_ptr owns the object).
    ThreadedWindPoint* rawWindPoint = localWindPoint.get();

    //queue the addition of the wind point to the scene in the main thread.
    //Qt requires GUI-related operations (like adding items to a scene) to happen in the main thread,
    //so we use invokeMethod with QueuedConnection to ensure thread safety.
    QMetaObject::invokeMethod(scene, [scene, rawWindPoint]() {
        scene->addItem(rawWindPoint);
    }, Qt::QueuedConnection);

    //safely move the ownership of the wind point to the target vector (windPoints).
    {
        //lock_guard<mutex> lock(shapeMutex);//lock the mutex to prevent concurrent access to the targetVector.
        targetVector.push_back(move(localWindPoint));//transfer ownership of the wind point to the vector.
    }//mutex is automatically unlocked here when lock_guard goes out of scope.
}

//run thread for each object of this type
inline void spawnThreadedWindPoint(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    shapeThreads.emplace_back(createThreadedWindPointThread, scene, posX, posY, radius, ref(targetVector));
}

//Template function to create a shape in a separate thread
//T: The type of shape (likeQGraphicsRectItem or QGraphicsEllipseItem)
//arguments are; scene, dimensions, color, and position, and specify the target vector to store it
template<typename T>
void createShapeThread(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY,float mass,float friction, vector<unique_ptr<T>>& targetVector) {
    auto localShape = std::make_unique<T>(0, 0, width, height,mass, friction);//Create a unique pointer to a new shape with specified width and height
    localShape->setBrush(color);
    localShape->setPos(posX, posY);

    //Use a raw pointer for adding to scene, then move ownership to vector
    T* rawShape = localShape.get();

    //Queue the addition of the polygon to the scene in the main thread (Qt GUI operations must occur in the main thread)    
    //Create etiquere for mass,friction, dx and dy display
    QMetaObject::invokeMethod(scene, [scene, rawShape, posX, posY, mass, friction]() {
        QGraphicsTextItem* label = new QGraphicsTextItem();
        label->setDefaultTextColor(Qt::black);
        label->setPlainText(QString("dx: 0\ndy: 0\nMass: %1\nFriction: %2").arg(mass).arg(friction));
        label->setPos(posX, posY - 70);//20px above current object...
        label->setZValue(20);//layer over object
        scene->addItem(rawShape);//add object
        scene->addItem(label);//add its etiquette
        qDebug() << "Etiquette added on pos:" << posX << "," << posY << "with the text:" << label->toPlainText();
        rawShape->setLabel(label);
    }, Qt::QueuedConnection);//in main thread - thats why i use this function

    //Move ownership to the vector after queuing the addition
    {
        //lock_guard<mutex> lock(shapeMutex);//lock the mutex to prevent concurrent access
        targetVector.push_back(std::move(localShape));//put to target vector of type(circle etc.)
    }
}

//Template function to spawn a shape by launching a thread
//T: The type of shape to spawn
//Creates a thread that calls createShapeThread with the given parameters
template<typename T>
void spawnShape(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY,float mass, float friction, vector<unique_ptr<T>>& targetVector) {

    //Add a new thread to the shapeThreads vector, passing the createShapeThread function and its arguments
    shapeThreads.emplace_back(createShapeThread<T>, scene, width, height, color, posX, posY,mass, friction, ref(targetVector));//ref - pass the reference to exact vector we cant
}

//Function to join all active threads in shapeThreads
inline void joinThreads() {
    for (auto& t : shapeThreads) {//iterate throw vector with threads
        if (t.joinable()) {
            t.join();
        }
    }
}

#endif // SHAPECREATOR_H
