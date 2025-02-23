#include "mainwindow.h"
#include "menupage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuPage menObj;
    menObj.show();

    return a.exec();
}
