#ifndef THREADEDWINDPOINT_H
#define THREADEDWINDPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <memory>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>

using namespace std;

//forward declaration to avoid recursive adding of .h files
class PhysicalRectItem;
class PhysicalEllipseItem;
class PhysicalPolygonItem;

extern mutex shapeMutex;

class ThreadedWindPoint : public QGraphicsEllipseItem {
public:
    ThreadedWindPoint(float x, float y, float radius, float windRadius, float maxForce, QGraphicsScene* scene);

    //put wint impact on all of the passed object in these vectors
    void applyWindForce(vector<unique_ptr<PhysicalRectItem>>& squares,
                        vector<unique_ptr<PhysicalRectItem>>& rectangles,
                        vector<unique_ptr<PhysicalEllipseItem>>& circles,
                        vector<unique_ptr<PhysicalPolygonItem>>& triangles,
                        vector<unique_ptr<PhysicalPolygonItem>>& trapezes,
                        const vector<unique_ptr<ThreadedWindPoint>>& windPoints);

    QGraphicsTextItem* getLabel() const { return label; }//method to get label(force and radius)
    void setLabel(QGraphicsTextItem* l) { label = l; }//method to set current label

private:
    float windRadius;//max distance of wind strength working on other object
    float maxForce;//max wind force(it is when object is on wind point - than further, than weeker)
    QGraphicsScene* scene;//scene
    QGraphicsTextItem* label;//label to display radius and MaxForce of winpoint
};

#endif // THREADEDWINDPOINT_H
