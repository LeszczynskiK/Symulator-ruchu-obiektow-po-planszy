#include <QPen>
#include <QBrush>
#include <memory>
#include <cmath>

#include "windpoint.h"

using namespace std;

WindPoint::WindPoint(float x, float y, float radius, QGraphicsScene* scene)
    : QGraphicsEllipseItem(x - radius / 2, y - radius / 2, radius, radius),//set start position(1st and 2nd argument), and set x and y radius size of ellipse
    windRadius(1500),//example wind width of having impacts on objects
    maxForce(20),//max push force value(in newton N) - start value(later will be calculated on force variable)
    scene(scene)//scene name
{
    setBrush(QBrush(Qt::yellow));//interior colour of wind object
    setPen(QPen(Qt::black));//frame of wind object
    scene->addItem(this);
    setZValue(1);//above other items *this is about layer)
    qDebug() << "WindPoint created at:" << x << "," << y;//to check becouse i have problems with visibility of windblock
}

void WindPoint::applyWindForce(vector<unique_ptr<QGraphicsRectItem>>& squares,//get vectors which have vectors of created object of any type
                               vector<unique_ptr<QGraphicsRectItem>>& rectangles,
                               vector<unique_ptr<QGraphicsEllipseItem>>& circles,
                               vector<unique_ptr<QGraphicsPolygonItem>>& triangles,
                               vector<unique_ptr<QGraphicsPolygonItem>>& trapezes)
{
    //Calculate the center position of the wind point (top-left pos + half of width/height)
    QPointF windPos = pos() + QPointF(rect().width() / 2, rect().height() / 2);
    //pos() - circle is based on rectangle(it return positon of left-top corner)
    //QPointF is variable which has 2 variables inside(x,y)
    //so we get x and y width and height of circle(circle inside rectangle(becouse circle base on rectagnle(
    // /2 becouse we have 2*radius, so we need to get only r x r ( so get interior coordinates of circle)

    //Lambda function to apply wind force to a given vector of items (generic for all types)
    //all of the captured values by [] are captured by reference
    auto applyForceToItems = [&](auto& items) {//loop throw the each item in provided vector
        for (auto& item : items) {
            QPointF itemPos = item->pos() + QPointF(item->boundingRect().width() / 2, item->boundingRect().height() / 2);//count middle point of item

            //Calculate the distance between wind point and item using hypot
            //hypot(x, y) computes the Euclidean distance: sqrt(x^2 + y^2)
            //x = difference in X coords, y = difference in Y coords
            //this gives the straight-line distance from the wind point to the item's center
            float distance = hypot(windPos.x() - itemPos.x(), windPos.y() - itemPos.y());

            //Check if the item is within the wind's radius and not at the exact same position (avoid division by zero)
            if (distance < windRadius && distance > 0) {

                //Calculate the force of the wind: stronger when closer, weaker when farther (non linear decrease)
                //if closer to object, then stronger power is
                float force = maxForce * (1 - (distance * distance) / (windRadius * windRadius));

                //Calculate the angle between wind point and item using stan2
                //atan2(y, x) returns the angle (in radians) from the positive X-axis to the point (x, y)
                //y = difference in Y coords, x = difference in X coords
                //This gives the direction from the wind point to the item, ranging from -pi to pi
                float angle = atan2(itemPos.y() - windPos.y(), itemPos.x() - windPos.x());

                //Calculate the horizontal (x) and vertical (y) components of the force using trigonometry
                float dx = force * cos(angle);//Movement along the X-axis
                float dy = force * sin(angle);//Movement along the Y-axis

                //Move the item by the calculated amounts (dx, dy) to simulate the wind effect
                item->moveBy(dx, dy);
            }
        }
    };

    //apply the wind force to each type of object by calling the lambda function
    applyForceToItems(squares);//Apply to all squares
    applyForceToItems(rectangles);//Apply to all rectangles
    applyForceToItems(circles);//Apply to all circles
    applyForceToItems(triangles);//Apply to all triangles
    applyForceToItems(trapezes);//Apply to all trapezoids
}
