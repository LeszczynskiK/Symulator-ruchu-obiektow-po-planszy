#ifndef THREADEDWINDPOINT_H
#define THREADEDWINDPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <memory>
#include <vector>
#include <cmath>

using namespace std;

class ThreadedWindPoint : public QGraphicsEllipseItem {
public:
    ThreadedWindPoint(float x, float y, float radius, QGraphicsScene* scene);

    //put wint impact on all of the passed object in these vectors
    void applyWindForce(vector<unique_ptr<QGraphicsRectItem>>& squares,
                        vector<unique_ptr<QGraphicsRectItem>>& rectangles,
                        vector<unique_ptr<QGraphicsEllipseItem>>& circles,
                        vector<unique_ptr<QGraphicsPolygonItem>>& triangles,
                        vector<unique_ptr<QGraphicsPolygonItem>>& trapezes);

private:
    float windRadius;//max distance of wind strength working on other object
    float maxForce;//max wind force(it is when object is on wind point - than further, than weeker)
    QGraphicsScene* scene;//scene
};

#endif // THREADEDWINDPOINT_H
