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

//Mutex for thread-safe access to shared resources
extern std::mutex shapeMutex;//extern becouse we declarated this in ShapeCreator.h

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
}

//Function to remove shapes and their associated threads
template <typename T>
void removeShapes(QGraphicsScene* scene, std::vector<std::thread>& shapeThreads, std::vector<std::unique_ptr<QGraphicsItem>>& shapeObjects) {
    //Synchronize access to shapeObjects and shapeThreads
    std::lock_guard<std::mutex> lock(shapeMutex);

    //Remove shapes from the scene and delete them
    for (auto& shape : shapeObjects) {
        scene->removeItem(shape.get());//Remove the shape from the scene
        shape.reset();//Reset the unique pointer, which deletes the object
    }
    shapeObjects.clear();//Clear the vector holding the shapes

    //Join all threads
    for (auto& t : shapeThreads) {
        if (t.joinable()) {
            t.join();//Ensure all threads are joined before exiting
        }
    }
    shapeThreads.clear();//Clear the vector of threads
}

#endif // SHAPEREMOVER_H
