#ifndef INFORMATIONS_H
#define INFORMATIONS_H


#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>

#include "menupage.h"
#include "mainwindow.h"

class Informations : public QWidget {
    Q_OBJECT

private slots:
    void paintEvent(QPaintEvent *event);//background setter
    void exitApp();
    void backToMenu();//go back to 1st page
    void startSimul();//go to simulation page


public:
    Informations(QWidget *parent = nullptr);
private:
    QPixmap background;//Background txt
    QPushButton *exitAppButton;
    QPushButton *menuButton;//go to welcome page button
    QPushButton *startSimulButton;//go to simulaton page button
    QLabel *instructionLabel;//place to put instruction(as string type)
};

#endif // INFORMATIONS_H
