#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>

//abstract class
class PhysicalObject {//this is a class which will characterise physical values of any object(except windpoint)
public:
    PhysicalObject(float m, float f):mass(m), friction(f),label(nullptr){}//this is only sketch for more detailed values
    virtual ~PhysicalObject(){ delete label;}//so abstract destructor to avoid memory leakage

    float getMass() const { return mass; }//get mass from QLineEdit
    void setMass(float m) { mass = m; }//set mass to object
    float getFriction() const { return friction; }//get friction from QLineEdit
    void setFriction(float f) { friction = f; }//set friction to object

    virtual QGraphicsItem* getGraphicsItem() = 0;//abstract method to get QGraphicsItem
    virtual float getSurfaceArea() const = 0;//virtual method for surface area(will be overwritten by exact type of objec)

    QGraphicsTextItem* getLabel() const { return label; }
    void setLabel(QGraphicsTextItem* l) { label = l; }

private:
    float mass;//mass in kg (like 1.0 kg)
    float friction;//friction is between 0 and 1 value(1 is max friction, 0 is no friction)
    QGraphicsTextItem* label;//etiquete for  dx, dy, mass, friction displaying
};

#endif // PHYSICALOBJECT_H
