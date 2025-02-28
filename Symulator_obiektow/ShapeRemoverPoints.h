#ifndef SHAPEREMOVERPOINTS_H
#define SHAPEREMOVERPOINTS_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
using namespace std;

extern std::mutex shapeMutex;//Shared mutex
extern vector<thread> shapeThreads;//Shared thread pool
extern vector<unique_ptr<QGraphicsPolygonItem>> triangles;//Vector for triangles
extern vector<unique_ptr<QGraphicsPolygonItem>> trapezes;//Vector for trapezoids

//Template function to remove polygon shapes from the scene and clean up memory
//T: The type of shape (typically QGraphicsPolygonItem)
//Parameters: The scene to remove from and the vector containing the polygon shapes to delete
template<typename T>
void removePolygonShapes(QGraphicsScene* scene, vector<unique_ptr<T>>& shapeVector) {
    lock_guard<mutex> lock(shapeMutex);//Lock the mutex to ensure thread-safe access to the shape vector

    //Iterate over all polygon shapes in the vector and remove them
    for (auto& shape : shapeVector) {
        scene->removeItem(shape.get());// emove the polygon shape from the scene using its raw pointer
        shape.reset();//Reset the unique pointer, which deletes the shape object and frees memory
    }
    shapeVector.clear();//Clear the vector to remove all (now null) unique pointers
}

#endif // SHAPEREMOVERPOINTS_H
