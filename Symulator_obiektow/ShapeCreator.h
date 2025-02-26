#ifndef SHAPECREATOR_H
#define SHAPECREATOR_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMetaObject>//to make possible working on threads(main functionality is on main thread, rest on added ones)
#include <QCoreApplication>
#include <QBrush>
#include <QColor>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
using namespace std;

vector<thread> shapeThreads;//vector to keep object of type thread(collect object created - each in separated thread)
vector<unique_ptr<QGraphicsItem>> shapeObjects;//Keep created object(of QGraphicsItem type) in vector(by mechanism of smart pointers)
mutex shapeMutex;//mutex to block access to protected fields of threads(block access of all threads in one moment)

//function only to create shape(not return or spawn)
template<typename T>//T will be type of shape(RectItem, EllipseItem) - can use only for figures which do not base on hand-written points
void createShapeThread(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY) {
    //Create new figure object(arguments are the base of object - it will personalize different type)
    auto localShape = std::make_unique<T>(0, 0, width, height);//object is based on smart pointer(2 last arguments define size of figure)
    localShape->setBrush(color);//add color to figure
    localShape->setPos(posX, posY);//set position of figure on scene

    QMetaObject::invokeMethod(scene, [scene, shape = move(localShape)]() {
        scene->addItem(shape.get());//Add shape to scene in the main thread
        {
            lock_guard<mutex> lock(shapeMutex);//Lock access to shared resource
            shapeObjects.push_back(std::unique_ptr<QGraphicsItem>(std::move(shape).release()));//Store the shape object
        }
    }, Qt::QueuedConnection);//Qt::QueuedConnection ensures this is executed in the main thread
}

//function to spawn created object in createShapeThread function
template<typename T>
void spawnShape(QGraphicsScene* scene, qreal width, qreal height, QColor color, qreal posX, qreal posY) {
    shapeThreads.emplace_back(createShapeThread<T>, scene, width, height, color, posX, posY);
    //shapeThreads is vector of thread type.
    //emplace_back - create new object of thread type indirectly in this vector
    //createShapeThread<T> is function which is turned on in new thread
    //T is type which will be called with exact type(Circle etc.)
    //rest of arguments will be given to this function(to create object with arguments on thread)

    //emplace_back is better because it creates an object indirectly in vector(if we used push_back etc
    //we should firstly create object and then move it to vector
    //with emplace_back we skip this part - we create object indirectly in vector
}

//Function to properly join threads before exiting the program
void joinThreads() {
    for (auto& t : shapeThreads) {
        if (t.joinable()) {
            t.join();//Ensure all threads are joined before exiting
        }
    }
}

#endif // SHAPECREATOR_H
