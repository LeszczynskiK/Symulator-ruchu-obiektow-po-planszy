#include "menupage.h"
#include "mainwindow.h"
#include "informations.h"



MenuPage::MenuPage(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Main menu ");

    const int x =1366;//x window size
    const int y=768;//y window size

    setFixedSize(x, y);//Set window size

    //Background image
    background = QPixmap("/home/krzysiek89/Desktop/QT_aplikacje/Symulator_ruchu_obiektow/Symulator_obiektow/MenuMain.png").scaled(x, y);


    QFont font;
    font.setPointSize(24);//Font size

    //Initialize buttons
    int x_pos=20;
    int gap=30;
    int y_pos = 510;
    int y_siz =100;
    int x_size=420;

    informationButton = new QPushButton("Information", this);//go to main menu
    informationButton->setFont(font);
    informationButton->setGeometry(x_pos, y_pos-y_siz-gap, x_size, y_siz);
    informationButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(informationButton, &QPushButton::clicked, this, &MenuPage::showInformation);//if clicked, go back to menu page

    simulationButton = new QPushButton("Go to simulation page..", this);//go to simulation page button
    simulationButton->setFont(font);
    simulationButton->setGeometry(x_pos, y_pos, x_size, y_siz);
    simulationButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(simulationButton, &QPushButton::clicked, this, &MenuPage::goToSimulation);//if clicked, go to simulation page


    exitAppButton = new QPushButton("Exit app...", this);//exit app
    exitAppButton->setFont(font);
    exitAppButton->setGeometry(x_pos, y_pos+y_siz+gap, x_size, y_siz);
    exitAppButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(exitAppButton, &QPushButton::clicked, this, &MenuPage::exitApp);
}

void MenuPage::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);//Background
    QWidget::paintEvent(event);
}

void MenuPage::exitApp()
{
    this->close();//close this window
}

void MenuPage::goToSimulation()//go back to main menu
{
    MainWindow *simPage = new MainWindow();
    simPage->show();//display simulation page
    this->close();//close this page
}

void MenuPage::showInformation()//go back to main menu
{
    Informations *infPage = new Informations();
    infPage->show();//display simulation page
    this->close();//close this page
}
