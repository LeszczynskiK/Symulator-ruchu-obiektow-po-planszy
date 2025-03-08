#ifndef SHAPESPAWNER_H
#define SHAPESPAWNER_H

#include <QGraphicsScene>
#include <QLineEdit>
#include <QColor>
#include <algorithm>
using namespace std;


//for rectangle and elise
template<typename SpawnFunc, typename VectorType>//base on spawn functon and object type
void spawnWithProperties(QGraphicsScene* scene, QLineEdit* massInput, QLineEdit* frictionInput,
                         SpawnFunc& spawnFunc, VectorType& targetVector,
                         qreal width, qreal height, QColor color, int x, int y) {//here we have x and y to create shape
    float mass = 1.0f, friction = 0.5f;//if not typed any values in QLineEdit, use these one
    bool ok;//conversion succeed?

    //get mass value and limit it to range..
    float inputMass = massInput->text().toFloat(&ok);
    if (ok) mass = max(0.00001f, min(100000.0f, inputMass));

    //get friciton and limit range
    float inputFriction = frictionInput->text().toFloat(&ok);
    if (ok) friction = max(0.0f, min(1.0f, inputFriction));

    //call spaw function with arguments
    spawnFunc(scene, width, height, color, x, y, mass, friction, targetVector);

    //clear mass and fricion qLineEdits
    massInput->clear();
    frictionInput->clear();
}

//for shapes made by points connecting
template<typename SpawnFunc, typename VectorType>
void spawnPolygonWithProperties(QGraphicsScene* scene, QLineEdit* massInput, QLineEdit* frictionInput,
                                SpawnFunc& spawnFunc, VectorType& targetVector,
                                const QPolygonF& polygon, QColor color, int x, int y) {//here we have QPolygon to create shape
    float mass = 1.0f, friction = 0.5f;
    bool ok;

    float inputMass = massInput->text().toFloat(&ok);
    if (ok) mass = max(0.00001f, min(100000.0f, inputMass));

    float inputFriction = frictionInput->text().toFloat(&ok);
    if (ok) friction = max(0.0f, min(1.0f, inputFriction));

    //call spawn function
    spawnFunc(scene, polygon, color, x, y, mass, friction, targetVector);

    //clear text field
    massInput->clear();
    frictionInput->clear();
}

#endif // SHAPESPAWNER_H
