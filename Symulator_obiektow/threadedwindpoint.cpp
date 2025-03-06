#include "threadedwindpoint.h"
#include <QDebug>
#include "ShapeCreator.h"//for PhysicalRectItem and PhysicalEllipseItem
#include "ShapeCreatorPoints.h"//for PhysicalPolygonItem

using namespace std;

ThreadedWindPoint::ThreadedWindPoint(float x, float y, float radius, QGraphicsScene* scene)//set start position(1st and 2nd argument), and y radius size of ellipse
    : QGraphicsEllipseItem(-radius / 2, -radius / 2, radius, radius),//2 arguments are start pos (co left up corner is on this pos, radius is total size in x and y (diameter)
    windRadius(500),//example wind width of having impacts on objects
    maxForce(20),//max push force value(in newton N) - start value(later will be calculated on force variable)
    scene(scene)//scene name
{
    setBrush(QBrush(Qt::yellow));//interior colour of wind object
    setPen(QPen(Qt::black));//frame of wind object
    setPos(x,y); //Set the position of the item
    setZValue(10);//Ensure WindPoint is above other items
    qDebug() << "WindPoint created at (x, y):" << x << "," << y
             << "Initial pos():" << pos()
             << "Rect:" << rect()
             << "Is Visible:" << isVisible() << "Opacity:" << opacity();
}

void ThreadedWindPoint::applyWindForce(vector<unique_ptr<PhysicalRectItem>>& squares,//get vectors which have vectors of created object of any type
                               vector<unique_ptr<PhysicalRectItem>>& rectangles,
                               vector<unique_ptr<PhysicalEllipseItem>>& circles,
                               vector<unique_ptr<PhysicalPolygonItem>>& triangles,
                               vector<unique_ptr<PhysicalPolygonItem>>& trapezes)
{
    QPointF windPos = pos();//mid of windPoint is indirectly in pos(), becouse rect is relatie to 0,0

    qDebug() << "WindPoint pos():" << pos() << "Rect:" << rect() << "Calculated center:" << windPos
             << "Is Visible:" << isVisible() << "Opacity:" << opacity()
             << "Scene Rect:" << scene->sceneRect();

    //pos() - circle is based on rectangle(it return positon of left-top corner)
    //QPointF is variable which has 2 variables inside(x,y)
    //so we get x and y width and height of circle(circle inside rectangle(becouse circle base on rectagnle(
    // /2 becouse we have 2*radius, so we need to get only r x r ( so get interior coordinates of circle)

    //Lambda function to apply wind force to a given vector of items (generic for all types)
    //all of the captured values by [] are captured by reference
    auto applyForceToItems = [&](auto& items) {//loop throw the each item in provided vector
        for (auto& item : items) {

            PhysicalObject* obj = dynamic_cast<PhysicalObject*>(item.get());//cast item to obj (different type of pointer)
            if (!obj) continue;//if cast do not suceed...

            QPointF itemPos = item->pos() + QPointF(item->boundingRect().width() / 2, item->boundingRect().height() / 2);//count middle point of item

            //Calculate the distance between wind point and item using hypot
            //hypot(x, y) computes the Euclidean distance: sqrt(x^2 + y^2)
            //x = difference in X coords, y = difference in Y coords
            //this gives the straight-line distance from the wind point to the item's center
            float distance = hypot(windPos.x() - itemPos.x(), windPos.y() - itemPos.y());
            qDebug() << "Object center at:" << itemPos << "Distance:" << distance;

            //Check if the item is within the wind's radius and not at the exact same position (avoid division by zero)
            if (distance < windRadius && distance > 0) {

                //Calculate the force of the wind: stronger when closer, weaker when farther (non linear decrease)
                //if closer to object, then stronger power is
                float force = maxForce * (1 - (distance * distance) / (windRadius * windRadius));

                //Calculate the angle between wind point and item using stan2(direction to push away)
                //atan2(y, x) returns the angle (in radians) from the positive X-axis to the point (x, y)
                //y = difference in Y coords, x = difference in X coords
                //This gives the direction from the wind point to the item, ranging from -pi to pi
                float angle = atan2(windPos.y() - itemPos.y(), windPos.x() - itemPos.x());

                float mass_value = dynamic_cast<PhysicalObject*>(item.get())->getMass();//mass value
                float acceleration = force / mass_value;//a = F / m

                //Calculate the horizontal (x) and vertical (y) components of the force using trigonometry

                //dx and dy depends on:force, mass and angle(wirection wind vs object)
                float dx = -acceleration * cos(angle);//Movement along the X-axis(nevative to push away)
                float dy = -acceleration * sin(angle);//Movement along the Y-axis(nevative to push away)

                //multiply by 1-f(friction impact on object)
                float friction = obj->getFriction();
                dx *= (1.0f - friction);
                dy *= (1.0f - friction);

                //Move the item by the calculated amounts (dx, dy) to simulate the wind effect
                item->moveBy(dx, dy);

                //actualise etiquere over object(with mass etc..)
                QGraphicsTextItem* label = obj->getLabel();
                if (label) {
                    QMetaObject::invokeMethod(scene, [label, dx, dy, mass_value, friction, itemPos = item->pos()]() {
                        label->setPlainText(QString("dx: %1\ndy: %2\nMass: %3\nFriction: %4")
                                                .arg(dx, 0, 'f', 2)//dx value
                                                .arg(dy, 0, 'f', 2)//dy value
                                                .arg(mass_value, 0, 'f', 2)//mass of object value
                                                .arg(friction, 0, 'f', 2));//friction of object value
                        label->setPos(itemPos.x(), itemPos.y() - 20);//actualise etiquete(this will be as always 20px above object)
                    }, Qt::QueuedConnection);//in main thread
                }
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
