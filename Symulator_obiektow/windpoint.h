#ifndef WINDPOINT_H
#define WINDPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <vector>
#include <memory>
using namespace std;


class WindPoint : public QGraphicsEllipseItem {//wind point is elipse shape
public:
    WindPoint(float x, float y, float radius, QGraphicsScene* scene);//x and y is positon where wind block will be placed, radius i r of wind(elipse item shape)

    //this method include 5 vectors which each one has created object of any type(which is currently created)
    //and have to pun any impact on object(square, circle etc - these class have object and each object
    //will have pressure on itseld - depends of side, width to wind etc...)
    void applyWindForce(vector<unique_ptr<QGraphicsRectItem>>& squares,
                        vector<unique_ptr<QGraphicsRectItem>>& rectangles,
                        vector<unique_ptr<QGraphicsEllipseItem>>& circles,
                        vector<unique_ptr<QGraphicsPolygonItem>>& triangles,
                        vector<unique_ptr<QGraphicsPolygonItem>>& trapezes);//keep objects in vectors

private:
    float windRadius;//max distance of wind having impact on object
    float maxForce;//max push force of wind
    QGraphicsScene* scene;//scene object
};

#endif // WINDPOINT_H
