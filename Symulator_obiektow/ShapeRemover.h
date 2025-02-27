#ifndef SHAPEREMOVER_H
#define SHAPEREMOVER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include <algorithm>//for find function
using namespace std;

//Mutex for thread-safe access to shared resources
extern std::mutex shapeMutex;//extern becouse we declarated this in ShapeCreator.h

//vectors to keep objects in
extern vector<thread> shapeThreads;
extern vector<unique_ptr<QGraphicsRectItem>> squares;
extern vector<unique_ptr<QGraphicsRectItem>> rectangles;
extern vector<unique_ptr<QGraphicsEllipseItem>> circles;
extern vector<unique_ptr<QGraphicsPolygonItem>> triangles;
extern vector<unique_ptr<QGraphicsPolygonItem>> trapezes;

//Template for object remover
template <typename T>
void removeObjects(QGraphicsScene* scene, QList<T*>& objects) {
    //Synchronize access to the shared resource (list of objects)
    std::lock_guard<std::mutex> lock(shapeMutex);

    for (auto& obj : objects) {
        scene->removeItem(obj);//Remove from the scene
        delete obj;//Delete the object from memory
    }
    objects.clear();//Clear the list
    //this functon is for triangle and trapeze(not made template s far, becouse it depends on points number)
}

//Function to remove shapes and their associated threads
template <typename T>
void removeShapes(QGraphicsScene* scene, std::vector<std::unique_ptr<T>>& shapeVector) {
    //Synchronize access to shapeObjects and shapeThreads
    std::lock_guard<std::mutex> lock(shapeMutex);

    //Remove shapes from the scene and delete them
    for (auto& shape : shapeVector) {
        scene->removeItem(shape.get());//Remove the shape from the scene
        shape.reset();//Reset the unique pointer, which deletes the object
    }
    shapeVector.clear();//Clear the vector holding the shapes
     //delete object from moved vector of exact type(circle etc.) , threaded are being cleared in clear threads in joinThreads
}

#endif // SHAPEREMOVER_H
