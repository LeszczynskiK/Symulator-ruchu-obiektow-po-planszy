#ifndef SHAPEREMOVER_H
#define SHAPEREMOVER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include "ShapeCreator.h"
using namespace std;

//Mutex for thread-safe access to shared resources
extern std::mutex shapeMutex;//extern becouse we declarated this in ShapeCreator.h

//vectors to keep objects in
extern vector<thread> shapeThreads;
extern vector<unique_ptr<PhysicalRectItem>> squares;
extern vector<unique_ptr<PhysicalRectItem>> rectangles;
extern vector<unique_ptr<PhysicalEllipseItem>> circles;

//Template function to remove shapes from the scene and clean up memory
//T: The type of shape (like QGraphicsRectItem or QGraphicsEllipseItem)
//Parameters: The scene to remove from and the vector containing the shapes to delete
template <typename T>
void removeShapes(QGraphicsScene* scene, std::vector<std::unique_ptr<T>>& shapeVector) {

    //Synchronize access to shapeObjects and shapeThreads
   // std::lock_guard<std::mutex> lock(shapeMutex);//Lock the mutex for thread safety

    //Remove shapes from the scene and delete them
    for (auto& shape : shapeVector) {
        scene->removeItem(shape.get());//Remove the shape from the scene
        shape.reset();//Reset the unique pointer, which deletes the object
    }
    shapeVector.clear();//Clear the vector to remove all (now null) unique pointers
}

#endif // SHAPEREMOVER_H
