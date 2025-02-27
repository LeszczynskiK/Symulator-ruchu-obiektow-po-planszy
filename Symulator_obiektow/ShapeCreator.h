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
}

#endif // SHAPECREATOR_H
