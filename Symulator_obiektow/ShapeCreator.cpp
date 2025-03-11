#include "ShapeCreator.h"

//extern - becouse defined is in mainwindow
extern vector<thread> shapeThreads;//threads vector definition
extern mutex shapeMutex;//mutex definition

//create windpoint (each in new thread)
void createThreadedWindPointThread(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius, float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    //create a unique pointer to a new ThreadedWindPoint object with the specified position and radius
    //ThreadedWindPoint is a custom class that simulates wind effects in the scene
    auto localWindPoint = make_unique<ThreadedWindPoint>(posX, posY, radius,windRadius, maxForce, scene);

    //get a raw pointer to the wind point for adding it to the scene (needed because unique_ptr owns the object).
    ThreadedWindPoint* rawWindPoint = localWindPoint.get();

    //queue the addition of the wind point to the scene in the main thread.
    //Qt requires GUI-related operations (like adding items to a scene) to happen in the main thread,
    //so we use invokeMethod with QueuedConnection to ensure thread safety.
    QMetaObject::invokeMethod(scene, [scene, rawWindPoint]() {
        scene->addItem(rawWindPoint);
    }, Qt::QueuedConnection);

    //safely move the ownership of the wind point to the target vector (windPoints).
    {
        lock_guard<mutex> lock(shapeMutex);//lock the mutex to prevent concurrent access to the targetVector.
        targetVector.push_back(move(localWindPoint));//transfer ownership of the wind point to the vector.
    }//mutex is automatically unlocked here when lock_guard goes out of scope.
}

//run thread for each object of this type
void spawnThreadedWindPoint(QGraphicsScene* scene, qreal posX, qreal posY, qreal radius,float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    shapeThreads.emplace_back(createThreadedWindPointThread, scene, posX, posY, radius,windRadius, maxForce, ref(targetVector));
}

//Function to join all active threads in shapeThreads
void joinThreads() {
    for (auto& t : shapeThreads) {//iterate throw vector with threads
        if (t.joinable()) {
            t.join();
        }
    }
}
