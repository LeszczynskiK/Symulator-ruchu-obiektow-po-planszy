#include "collisionhandler.h"

CollisionHandler::CollisionHandler(QGraphicsScene* scene) : scene(scene) {}

//helper function to check if two objects bump into each other
//takes two items and returns me if they collide
bool CollisionHandler::checkCollisionBetween(QGraphicsItem* item1, QGraphicsItem* item2) {
    if (!item1 || !item2 || item1 == item2) return false;//if any item is null or it’s the same object, no collision
    return item1->collidesWithItem(item2);//QT built in method to see if they collide
}

//helper function to extract dx, dy from label text(wind update dx and dy)
void CollisionHandler::extractVelocity(QGraphicsTextItem* label, float& dx, float& dy)
{
    if (!label) return;//if no label, just leave dx and dy as 0

    QString text = label->toPlainText();//get the text from the label
    QStringList lines = text.split("\n");//split it into lines (dx, dy, mass, friction) - \n is moment where new line is made

    dx = lines[0].mid(4).toFloat();//extract dx(mid(4) means skip "dx: "->skip 4 chars
    dy = lines[1].mid(4).toFloat();//extract dy
}

//helper function to update label with new dx, dy(keep the label in sync after collision object x object)
void CollisionHandler::updateLabel(QGraphicsTextItem* label, float dx, float dy, float mass, float friction)
{
    if (label){//if label exist
        label->setPlainText(QString("dx: %1\ndy: %2\nMass: %3\nFriction: %4")//set new values to label
                                .arg(dx, 0, 'f', 2)//new dx with 2 decimal places
                                .arg(dy, 0, 'f', 2)
                                .arg(mass, 0, 'f', 2)//keep the mass as is
                                .arg(friction, 0, 'f', 2));//keep the friction as is
    }
}

//template function to check collisions within the same type of shapes
template<typename T>
void CollisionHandler::checkCollisionsWithin(vector<unique_ptr<T>>& items)
{
    lock_guard<mutex> lock(collisionMutex);//thread save - accec to saved are in one time denied

    for (size_t i = 0; i < items.size(); ++i)//loop through all items in the vector
    {
        for (size_t j = i + 1; j < items.size(); ++j)//start from i+1 so I dont check the same pair twice or an object with itself
        {
            if (checkCollisionBetween(items[i].get(), items[j].get()))//if these two collide...
            {
                qDebug() << "Collision detected!\n";

                //get the physical objects to work with their speeds
                PhysicalObject* obj1 = dynamic_cast<PhysicalObject*>(items[i].get());
                PhysicalObject* obj2 = dynamic_cast<PhysicalObject*>(items[j].get());

                //extract dx, dy from labels
                float dx1 = 0.0f, dy1 = 0.0f, dx2 = 0.0f, dy2 = 0.0f;//preinitialise values
                extractVelocity(obj1->getLabel(), dx1, dy1);//grab speeds for first object
                extractVelocity(obj2->getLabel(), dx2, dy2);//grab speeds for second object

                //get masses - needed for realistic bounce based on physics lasw
                float m1 = obj1->getMass(), m2 = obj2->getMass();

                //calculate new velocities after collision - using elastic bounce with mass
                float newDx1 = ((m1 - m2) * dx1 + 2 * m2 * dx2) / (m1 + m2); //new x speed for obj1
                float newDy1 = ((m1 - m2) * dy1 + 2 * m2 * dy2) / (m1 + m2); //new y speed for obj1
                float newDx2 = ((m2 - m1) * dx2 + 2 * m1 * dx1) / (m1 + m2); //new x speed for obj2
                float newDy2 = ((m2 - m1) * dy2 + 2 * m1 * dy1) / (m1 + m2); //new y speed for obj2

                //update labels with new velocities(to kave new values (wind + collision impact)
                updateLabel(obj1->getLabel(), newDx1, newDy1, m1, obj1->getFriction());
                updateLabel(obj2->getLabel(), newDx2, newDy2, m2, obj2->getFriction());

                //move them a bit to avoid sticking together(use the new speeds)
                items[i]->moveBy(newDx1, newDy1);
                items[j]->moveBy(newDx2, newDy2);
            }
        }
    }
}

//template function to check collisions between two different types of shapes
template<typename T1, typename T2>
void CollisionHandler::checkCollisionsBetween(vector<unique_ptr<T1>>& items1, vector<unique_ptr<T2>>& items2)
{
    lock_guard<mutex> lock(collisionMutex);

    for (const auto& item1 : items1)//loop through all items of the first type
    {
        for (const auto& item2 : items2)//and all items of the second type
        {
            if (checkCollisionBetween(item1.get(), item2.get()))//if these to collide
            {
                qDebug() << "Collision detected!\n";

                //get the physical objects for physics fun
                PhysicalObject* obj1 = dynamic_cast<PhysicalObject*>(item1.get());
                PhysicalObject* obj2 = dynamic_cast<PhysicalObject*>(item2.get());

                //extract dx, dy from labels
                float dx1 = 0.0f, dy1 = 0.0f, dx2 = 0.0f, dy2 = 0.0f;
                extractVelocity(obj1->getLabel(), dx1, dy1); //speeds for first object
                extractVelocity(obj2->getLabel(), dx2, dy2); //speeds for second object

                //get masses - for the bounce calculation
                float m1 = obj1->getMass(), m2 = obj2->getMass();

                //calculate new velocities after collision - elastic bounce with mass
                float newDx1 = ((m1 - m2) * dx1 + 2 * m2 * dx2) / (m1 + m2); //new x speed for obj1
                float newDy1 = ((m1 - m2) * dy1 + 2 * m2 * dy2) / (m1 + m2); //new y speed for obj1
                float newDx2 = ((m2 - m1) * dx2 + 2 * m1 * dx1) / (m1 + m2); //new x speed for obj2
                float newDy2 = ((m2 - m1) * dy2 + 2 * m1 * dy1) / (m1 + m2); //new y speed for obj2

                //update labels with new velocities(to kave new values (wind + collision impact)
                updateLabel(obj1->getLabel(), newDx1, newDy1, m1, obj1->getFriction());
                updateLabel(obj2->getLabel(), newDx2, newDy2, m2, obj2->getFriction());

                //move them a bit to avoid sticking together(use the new speeds)
                item1->moveBy(newDx1, newDy1);
                item2->moveBy(newDx2, newDy2);
            }
        }
    }
}

//main function to check all collisions between my shapes
//combine all needed vectors here and then pass to mmain window function done method..
void CollisionHandler::checkCollisions(
    vector<unique_ptr<PhysicalRectItem>>& squares,//all my squares
    vector<unique_ptr<PhysicalRectItem>>& rectangles,//all my rectangles
    vector<unique_ptr<PhysicalEllipseItem>>& circles,//all my circles
    vector<unique_ptr<PhysicalPolygonItem>>& triangles,//all my triangles
    vector<unique_ptr<PhysicalPolygonItem>>& trapezes)//all my trapezes
{
    //checking collisions within the same type(square vs square etc...)
    checkCollisionsWithin(squares);//check all squares against each other
    checkCollisionsWithin(rectangles);//same for rectangles
    checkCollisionsWithin(circles);//and circles
    checkCollisionsWithin(triangles);//and triangles
    checkCollisionsWithin(trapezes);//and trapezes

    //checking collisions between different types - all possible pairs
    checkCollisionsBetween(squares, rectangles);//squares vs rectangles
    checkCollisionsBetween(squares, circles);//squares vs circles
    checkCollisionsBetween(squares, triangles);//squares vs triangles
    checkCollisionsBetween(squares, trapezes);//squares vs trapezes

    checkCollisionsBetween(rectangles, circles);//rectangles vs circles
    checkCollisionsBetween(rectangles, triangles);//rectangles vs triangles
    checkCollisionsBetween(rectangles, trapezes);//rectangles vs trapezes

    checkCollisionsBetween(circles, triangles);//circles vs triangles
    checkCollisionsBetween(circles, trapezes);//circles vs trapezes

    checkCollisionsBetween(triangles, trapezes);//triangles vs trapezes
}
