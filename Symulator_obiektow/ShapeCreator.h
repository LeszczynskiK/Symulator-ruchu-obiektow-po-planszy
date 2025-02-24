#ifndef SHAPECREATOR_H
#define SHAPECREATOR_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QColor>

template<typename T>//T will be type of shape(RectItem, EllipseItem) - can use only for figure which do not base on hand written points
T* createShape(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY) {

    //Create new figure object(arguments are the base of object - it will personalize different type)
    T* shape = new T(0, 0, width, height);//set size of figure
    shape->setBrush(color);//add colour of figure
    shape->setPos(posX, posY);//set position of figure on scene
    scene->addItem(shape);//add figure to scene
    return shape;//return object
}

#endif // SHAPECREATOR_H
