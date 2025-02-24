#ifndef SHAPEREMOVER_H
#define SHAPEREMOVER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>

//template for object remover
template <typename T>
void removeObjects(QGraphicsScene* scene, QList<T*>& objects) {//scene name and list (of any type of object name)
    for (auto& obj : objects) {//iterate throw list
        scene->removeItem(obj);//remove from scene
        delete obj;//delete from memory
    }
    objects.clear();//clear list
}

#endif // SHAPEREMOVER_H
