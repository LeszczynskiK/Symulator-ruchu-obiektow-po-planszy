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
#include <QGraphicsTextItem>
#include "physicalobject.h"//to add physical values to the objects
#include "threadedwindpoint.h"

using namespace std;

extern vector<thread> shapeThreads;//Shared thread pool
extern mutex shapeMutex;//A shared mutex to ensure thread-safe access to shared resources

//this is a class to specify all of the objects which base on polygon item(base on shapes made from points)
class PhysicalPolygonItem : public QGraphicsPolygonItem, public PhysicalObject {
public:
    PhysicalPolygonItem(const QPolygonF& polygon,float mass, float friction)
        : QGraphicsPolygonItem(polygon),PhysicalObject(mass, friction) {}
    QGraphicsItem* getGraphicsItem() override { return this; }

    //count area of polygon shape(depends of points number)
    float getSurfaceArea() const override {
        //we will use shoelace formula to calculate polygon area
        const QPolygonF& poly = polygon();
        float area = 0.0f;
        int n = poly.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            area += poly[i].x() * poly[j].y();
            area -= poly[j].x() * poly[i].y();
        }
        return fabs(area) / 2.0f;//Absolute value divided by 2
    }
};

//Template function to create a polygon shape in a separate thread
//T: The type of shape (like QGraphicsPolygonItem)
//Parameters define the polygon's geometry (via QPolygonF), color, position, and target vector
template<typename T>
void createPolygonShapeThread(QGraphicsScene* scene, const QPolygonF& polygon, QColor color, qreal posX, qreal posY,float mass, float friction, vector<unique_ptr<T>>& targetVector) {
    auto localShape = make_unique<T>(polygon,mass, friction);//Create a unique pointer to a new polygon shape using the provided QPolygonF
    localShape->setBrush(color);
    localShape->setPos(posX, posY);

    T* rawShape = localShape.get();//Get a raw pointer to the shape for scene addition

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

    //Safely transfer ownership of the polygon to the target vector
    {
        lock_guard<mutex> lock(shapeMutex);//Lock the mutex to prevent concurrent access to the target vector
        targetVector.push_back(move(localShape));//Move the unique pointer into the target vector (like triangles, trapezes)
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

inline void createWindPointThread(QGraphicsScene* scene, qreal posX, qreal posY, float radius, float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    auto localWindPoint = make_unique<ThreadedWindPoint>(posX, posY, radius, windRadius, maxForce, scene);
    ThreadedWindPoint* rawWindPoint = localWindPoint.get();

    //queue the addition of the wind point and its label to the scene in the main thread
    QMetaObject::invokeMethod(scene, [scene, rawWindPoint, posX, posY, maxForce, windRadius]() {
        //Create the label
        QGraphicsTextItem* label = new QGraphicsTextItem();
        label->setDefaultTextColor(Qt::black);
        label->setPlainText(QString("MaxForce: %1\nRadius: %2").arg(maxForce).arg(windRadius));
        label->setPos(posX, posY - 20);//Position it 20px above the wind point
        label->setZValue(20);//layer over wintpoint (make sure the visibility is ok)

        //Add the wind point and its label to the scene
        scene->addItem(rawWindPoint);
        scene->addItem(label);

        //Link the label to the wind point
        rawWindPoint->setLabel(label);
    }, Qt::QueuedConnection);

    //safely transfer ownership to the target vector
    {
        lock_guard<mutex> lock(shapeMutex);
        targetVector.push_back(move(localWindPoint));
    }
}

inline void spawnWindPoint(QGraphicsScene* scene, qreal posX, qreal posY, float radius, float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    shapeThreads.emplace_back(createWindPointThread, scene, posX, posY, radius, windRadius, maxForce, ref(targetVector));
}
#endif // SHAPECREATORPOINTS_H
