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

extern mutex shapeMutex;//mutex to ensure thread-safe access to shared resources for example vectors

//Declarations of functions, which are not templates:
void createThreadedWindPointThread(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius, //create object
                                   float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector);

void spawnThreadedWindPoint(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius,//spawn(show on scene) created object
                            float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector);

void joinThreads();//threads function manager

//Template function to create a shape in a separate thread
//T: The type of shape (likeQGraphicsRectItem or QGraphicsEllipseItem)
//arguments are; scene, dimensions, color, and position, and specify the target vector to store it
template<typename T>
void createShapeThread(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY,float mass,float friction, vector<unique_ptr<T>>& targetVector) {
    auto localShape = make_unique<T>(0, 0, width, height,mass, friction);//Create a unique pointer to a new shape with specified width and height
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
        lock_guard<mutex> lock(shapeMutex);//lock the mutex to prevent concurrent access
        targetVector.push_back(move(localShape));//put to target vector of type(circle etc.)
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

#endif // SHAPECREATOR_H
