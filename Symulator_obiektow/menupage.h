#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>

class MenuPage : public QWidget {
    Q_OBJECT

private slots:
    void paintEvent(QPaintEvent *event);//background setter
    void exitApp();
    void goToSimulation();//go to simulation page


public:
    MenuPage(QWidget *parent = nullptr);
private:
    QPixmap background;//Background txt
    QPushButton *exitAppButton;
    QPushButton *simulationButton;//go to simulation page
};

#endif // MENUPAGE_H
