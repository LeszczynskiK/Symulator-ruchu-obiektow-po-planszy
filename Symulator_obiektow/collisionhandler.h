#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <QGraphicsScene>
#include <QDebug>
#include <vector>
#include <memory>
#include <mutex>
#include "ShapeCreator.h"
#include "ShapeCreatorPoints.h"

using namespace std;

//this is my collision handler class to detect when shapes crash into each other(except windpoints)
class CollisionHandler {
public:
    CollisionHandler(QGraphicsScene* scene);//constructor with scene pointer(scene where actions happens)

    //main function to check collisions between all my shapes
    //I pass all vectors with objects here(except windpoints - they dont need to be checked )
    void checkCollisions(
        vector<unique_ptr<PhysicalRectItem>>& squares,//vector with all squares
        vector<unique_ptr<PhysicalRectItem>>& rectangles,//vector with all rectangles
        vector<unique_ptr<PhysicalEllipseItem>>& circles,//vector with all circles
        vector<unique_ptr<PhysicalPolygonItem>>& triangles,//vector with all triangles
        vector<unique_ptr<PhysicalPolygonItem>>& trapezes//vector with all trapezes
        );

private:
    QGraphicsScene* scene;//pointer to my scene where all shapes are drawn and moved
    mutex collisionMutex;//mutex to keep threads safe when checking collisions

    //helper function to check if two objects bump into each other
    //takes two QGraphicsItem pointers and returns true if they collide
    bool checkCollisionBetween(QGraphicsItem* item1, QGraphicsItem* item2);

    //template function to check collisions within the same type of shapes
    template<typename T>
    void checkCollisionsWithin(vector<unique_ptr<T>>& items);

    //template function to check collisions between two different types of shapes
    //like square vs circle or triangle vs trapeze etc...
    template<typename T1, typename T2>
    void checkCollisionsBetween(vector<unique_ptr<T1>>& items1, vector<unique_ptr<T2>>& items2);
    void extractVelocity(QGraphicsTextItem* label, float& dx, float& dy);//get dx and dy from label
    void updateLabel(QGraphicsTextItem* label, float dx, float dy, float mass, float friction);//update label after object x object impact(collision)
};

#endif // COLLISIONHANDLER_H
