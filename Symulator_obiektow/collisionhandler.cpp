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

                //Calculate the collision normal vector (direction of impact)
                QPointF pos1 = items[i]->pos();//Position of obj1
                QPointF pos2 = items[j]->pos();//Position of obj2
                QRectF rect1 = items[i]->boundingRect();//Bounding rectangle of obj1
                QRectF rect2 = items[j]->boundingRect();//Bounding rectangle of obj2

                //Calculate center points of both objects
                QPointF center1 = pos1 + QPointF(rect1.width() / 2, rect1.height() / 2);//Center of obj1
                QPointF center2 = pos2 + QPointF(rect2.width() / 2, rect2.height() / 2);//Center of obj2
                QPointF diff = center2 - center1;//vector from center1 to center2
                float distance = sqrt(diff.x() * diff.x() + diff.y() * diff.y());//Distance between centers

                if (distance == 0) continue;//Avoid division by zero if objects are exactly overlapping(object on object)

                /*
                Physics principles applied:

                - Conservation of Momentum: The total momentum of the system before and after
                  the collision remains constant. Given two objects with masses m1 and m2
                  and velocities v1 and v2, their momentum is conserved:
                  m1 * v1 + m2 * v2 = m1 * v1' + m2 * v2'

                - Conservation of Kinetic Energy (Elastic Collision): In a perfectly elastic
                  collision, kinetic energy is conserved:
                  0.5 * m1 * v1^2 + 0.5 * m2 * v2^2 = 0.5 * m1 * v1'^2 + 0.5 * m2 * v2'^2

                - Normal and Tangent Decomposition: Velocities are projected onto the normal
                  (collision direction) and tangent (perpendicular to normal) components.
                  - The normal component is updated using momentum conservation.
                  - The tangent component remains unchanged (assuming no friction).

                - Unit Normal and Tangent Vectors: The normal vector defines the axis along
                  which momentum exchange occurs, while the tangent vector remains unaffected.

                - Collision Response: New normal velocities are computed using the
                  1D elastic collision formula to maintain both momentum and kinetic energy.

                - Object Separation: To prevent objects from sticking together,
                  they are moved apart based on their masses and calculated overlap.
                */

                //Normal vector: direction of collision, normalized to unit length
                //This vector points from obj1 to obj2 and defines the axis along which momentum is exchanged
                QPointF normal(diff.x() / distance, diff.y() / distance);

                //Tangent vector: perpendicular to the normal vector (-y, x)
                //This vector defines the axis along which velocity is unaffected (no friction assumed)
                QPointF tangent(-normal.y(), normal.x());

                //Project velocities onto normal and tangent vectors using dot product
                float v1n = dx1 * normal.x() + dy1 * normal.y();//Normal component of obj1 velocity
                float v1t = dx1 * tangent.x() + dy1 * tangent.y();//Tangent component of obj1 velocity
                float v2n = dx2 * normal.x() + dy2 * normal.y();//Normal component of obj2 velocity
                float v2t = dx2 * tangent.x() + dy2 * tangent.y();//Tangent component of obj2 velocity

                //Calculate new velocities along the normal axis (elastic collision)
                //These formulas account for mass and ensure conservation of momentum and energy
                float newV1n = ((m1 - m2) * v1n + 2 * m2 * v2n) / (m1 + m2);//New normal velocity for obj1
                float newV2n = ((m2 - m1) * v2n + 2 * m1 * v1n) / (m1 + m2);//New normal velocity for obj2

                //Tangent velocities remain unchanged (no friction in this model)
                float newV1t = v1t;//Tangent velocity for obj1 stays the same
                float newV2t = v2t;//Tangent velocity for obj2 stays the same

                //Convert back to x and y components by combining normal and tangent contributions
                float newDx1 = newV1n * normal.x() + newV1t * tangent.x();//New x velocity for obj1
                float newDy1 = newV1n * normal.y() + newV1t * tangent.y();//New y velocity for obj1
                float newDx2 = newV2n * normal.x() + newV2t * tangent.x();//New x velocity for obj2
                float newDy2 = newV2n * normal.y() + newV2t * tangent.y();//New y velocity for obj2


                //Apply scaling factor to make the motion more visible (adjust as needed)
                float scale = 1.0f;//Scale factor to amplify velocities
                newDx1 *= scale;
                newDy1 *= scale;
                newDx2 *= scale;
                newDy2 *= scale;

                //Move objects immediately instead of updating labels with new velocities(before i added collision
                //effect to label and it was constantly movin with collision effect, I only want to move in once
                //when collision happen(move once and then let only wind affect)
                items[i]->moveBy(newDx1, newDy1);//Move obj1 once based on collision
                items[j]->moveBy(newDx2, newDy2);//Move obj2 once based on collision

                //Separate objects to prevent sticking
                //Calculate overlap and move objects apart along the normal vector
                float overlap = (rect1.width() + rect2.width()) / 2 - distance;//Approximate overlap distance
                if (overlap > 0) {
                    float moveFactor = overlap / (m1 + m2);//Distribute movement inversely proportional to mass
                    items[i]->moveBy(-normal.x() * moveFactor * m2, -normal.y() * moveFactor * m2);//Move obj1 away
                    items[j]->moveBy(normal.x() * moveFactor * m1, normal.y() * moveFactor * m1);//Move obj2 away
                }
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
                extractVelocity(obj1->getLabel(), dx1, dy1);//speeds for first object
                extractVelocity(obj2->getLabel(), dx2, dy2);//speeds for second object

                //get masses - for the bounce calculation
                float m1 = obj1->getMass(), m2 = obj2->getMass();

                //Calculate the collision normal vector (direction of impact)
                QPointF pos1 = item1->pos();//Position of obj1
                QPointF pos2 = item2->pos();//Position of obj2
                QRectF rect1 = item1->boundingRect();//Bounding rectangle of obj1
                QRectF rect2 = item2->boundingRect();//Bounding rectangle of obj2

                //Calculate center points of both objects
                QPointF center1 = pos1 + QPointF(rect1.width() / 2, rect1.height() / 2);//Center of obj1
                QPointF center2 = pos2 + QPointF(rect2.width() / 2, rect2.height() / 2);//Center of obj2
                QPointF diff = center2 - center1;//Vector from center1 to center2
                float distance = sqrt(diff.x() * diff.x() + diff.y() * diff.y());//Distance between centers

                if (distance == 0) continue;//Avoid division by zero if objects are exactly overlapping(object on object)

                //Normal vector: direction of collision, normalized to unit length
                //This vector points from obj1 to obj2 and defines the axis along which momentum is exchanged
                QPointF normal(diff.x() / distance, diff.y() / distance);

                //Tangent vector: perpendicular to the normal vector (-y, x)
                //This vector defines the axis along which velocity is unaffected (no friction assumed)
                QPointF tangent(-normal.y(), normal.x());

                //Project velocities onto normal and tangent vectors using dot product
                float v1n = dx1 * normal.x() + dy1 * normal.y();//Normal component of obj1 velocity
                float v1t = dx1 * tangent.x() + dy1 * tangent.y();//Tangent component of obj1 velocity
                float v2n = dx2 * normal.x() + dy2 * normal.y();//Normal component of obj velocity
                float v2t = dx2 * tangent.x() + dy2 * tangent.y();//Tangent component of obj2 velocity

                //Calculate new velocities along the normal axis (elastic collision)
                //These formulas account for mass and ensure conservation of momentum and energy
                float newV1n = ((m1 - m2) * v1n + 2 * m2 * v2n) / (m1 + m2);//New normal velocity for obj1
                float newV2n = ((m2 - m1) * v2n + 2 * m1 * v1n) / (m1 + m2);//New normal velocity for obj2

                //Tangent velocities remain unchanged (no friction in this model)
                float newV1t = v1t;//Tangent velocity for obj1 stays the same
                float newV2t = v2t;//Tangent velocity for obj2 stays the same

                //Convert back to x and y components by combining normal and tangent contributions
                float newDx1 = newV1n * normal.x() + newV1t * tangent.x();//New x velocity for obj1
                float newDy1 = newV1n * normal.y() + newV1t * tangent.y();//New y velocity for obj1
                float newDx2 = newV2n * normal.x() + newV2t * tangent.x();//New x velocity for obj2
                float newDy2 = newV2n * normal.y() + newV2t * tangent.y();//New y velocity for obj2

                //Apply scaling factor to make the motion more visible (adjust as needed)
                float scale = 1.0f;//Scale factor to amplify velocities
                newDx1 *= scale;
                newDy1 *= scale;
                newDx2 *= scale;
                newDy2 *= scale;

                //Move objects immediately instead of updating labels with new velocities(before i added collision
                //effect to label and it was constantly movin with collision effect, I only want to move in once
                //when collision happen(move once and then let only wind affect)
                item1->moveBy(newDx1, newDy1);//Move obj1 once based on collision
                item2->moveBy(newDx2, newDy2);//Move obj2 once based on collision

                //Separate objects to prevent sticking
                //Calculate overlap and move objects apart along the normal vector
                float overlap = (rect1.width() + rect2.width()) / 2 - distance;//Approximate overlap distance
                if (overlap > 0) {
                    float moveFactor = overlap / (m1 + m2);//Distribute movement inversely proportional to mass
                    item1->moveBy(-normal.x() * moveFactor * m2, -normal.y() * moveFactor * m2);//Move obj1 away
                    item2->moveBy(normal.x() * moveFactor * m1, normal.y() * moveFactor * m1);//Move obj2 away
                }
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
