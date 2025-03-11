#include "ShapeCreatorPoints.h"

//extern definitoon - 1st defiition is in mainwindow
extern vector<thread> shapeThreads;
extern mutex shapeMutex;

void createWindPointThread(QGraphicsScene* scene, qreal posX, qreal posY, float radius, float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    auto localWindPoint = make_unique<ThreadedWindPoint>(posX, posY, radius, windRadius, maxForce, scene);
    ThreadedWindPoint* rawWindPoint = localWindPoint.get();

    //queue the addition of the wind point and its label to the scene in the main thread
    QMetaObject::invokeMethod(scene, [scene, rawWindPoint, posX, posY, maxForce, windRadius]() {
        //Create the label
        QGraphicsTextItem* label = new QGraphicsTextItem();
        label->setDefaultTextColor(Qt::black);
        label->setPlainText(QString("MaxForce: %1\nRadius: %2").arg(maxForce).arg(windRadius));
        label->setPos(posX, posY - 20);//Position it 20px above the wind point
        label->setZValue(20);//layer over wintpoint (make sure the visibility is ok)

        //Add the wind point and its label to the scene
        scene->addItem(rawWindPoint);
        scene->addItem(label);

        //Link the label to the wind point
        rawWindPoint->setLabel(label);
    }, Qt::QueuedConnection);

    //safely transfer ownership to the target vector
    {
        lock_guard<mutex> lock(shapeMutex);
        targetVector.push_back(move(localWindPoint));
    }
}

void spawnWindPoint(QGraphicsScene* scene, qreal posX, qreal posY, float radius, float windRadius, float maxForce, vector<unique_ptr<ThreadedWindPoint>>& targetVector) {
    shapeThreads.emplace_back(createWindPointThread, scene, posX, posY, radius, windRadius, maxForce, ref(targetVector));
}
