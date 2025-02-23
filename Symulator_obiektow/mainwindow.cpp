#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Shakes physics simulation");

    const int x =1366;//x window size
    const int y=768;//y window size

    setFixedSize(x, y);//Set window size

    //Background image
    background = QPixmap("/home/krzysiek89/Desktop/QT_aplikacje/Symulator_ruchu_obiektow/Symulator_obiektow/AppPage.jpg").scaled(x, y);

    //Frame based on QLabel(frame with transparent background and "frame" around)
    frame = new QLabel(this);
    frame->setGeometry(0, 0, 1366, 663);
    frame->setStyleSheet("background-color: transparent; border: 10px solid black;");

    QFont font;
    font.setPointSize(16);//Font size

    //Initialize buttons
    int x_pos=5;
    int gap=10;
    int y_pos = 670;
    int y_siz =40;
    int x_size=145;

    menuButton = new QPushButton("Menu", this);//go to main menu
    menuButton->setFont(font);
    menuButton->setGeometry(x_pos, y_pos, x_size, y_siz);
    menuButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::backToMenu);//if clicked, go back to menu page

    exitAppButton = new QPushButton("Exit", this);//exit app
    exitAppButton->setFont(font);
    exitAppButton->setGeometry(x_pos, y_pos+y_siz+gap, x_size, y_siz);
    exitAppButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(exitAppButton, &QPushButton::clicked, this, &MainWindow::exitApp);

}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);//Background
    QWidget::paintEvent(event);
}

void MainWindow::exitApp()
{
    this->close();//close this window
}

void MainWindow::backToMenu()//go back to main menu
{
    MenuPage *menPage = new MenuPage();
    menPage->show();//display menu page
    this->close();//close this page
}
