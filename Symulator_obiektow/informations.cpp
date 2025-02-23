#include "informations.h"

Informations::Informations(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Informations");

    const int x =1366;//x window size
    const int y=768;//y window size

    setFixedSize(x, y);//Set window size

    //Background image
    background = QPixmap("/home/krzysiek89/Desktop/QT_aplikacje/Symulator_ruchu_obiektow/Symulator_obiektow/Informations.png").scaled(x, y);


    QFont font;
    font.setPointSize(16);//Font size

    //Initialize buttons
    int x_pos=5;
    int gap=10;
    int y_pos = 670;
    int y_siz =40;
    int x_size=145;

    // Add instruction text
    instructionLabel = new QLabel(this);

    // using \n before " give us possibility to use " as symbol, not as special
    QString instructions = "The goal of this project is to create a physics simulator that models the movement of particles in a 2D space. "
                           "Particles of different shapes (e.g., circles, squares, triangles) move across the board, responding to \"wind gusts\" "
                           "and bouncing off the walls of a frame that serves as the boundary of the board.\n\n\n"
                           "Particles: Different shapes of particles (e.g., circle, square, triangle) are represented by different classes inheriting from a common base class. "
                           "Particles have various properties (mass, shape, velocity, forces acting on them).\n\n\n"
                           "Forces: Particles respond to forces, such as \"wind gusts\" in selected points on the board.\n\n\n"
                           "Bouncing: Particles bounce off the boundaries of the board according to the principle of reflection, changing the direction of velocity.\n";


    //setting of instruction(text will be in semi-transparent background frame)
    instructionLabel->setText(instructions);
    instructionLabel->setWordWrap(true);//Enable text wrapping
    instructionLabel->setGeometry(160, 50, 1050, 600);
    instructionLabel->setStyleSheet("background-color: rgba(255, 255, 255, 180);color: black; border: 2px solid black; padding: 10px;");
    instructionLabel->setFont(QFont("Arial", 20));


    startSimulButton = new QPushButton("Simulation", this);//go to main menu
    startSimulButton->setFont(font);
    startSimulButton->setGeometry(x_pos, y_pos-y_siz-gap, x_size, y_siz);
    startSimulButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(startSimulButton, &QPushButton::clicked, this, &Informations::startSimul);//if clicked, go back to menu page

    menuButton = new QPushButton("Menu", this);//go to main menu
    menuButton->setFont(font);
    menuButton->setGeometry(x_pos, y_pos, x_size, y_siz);
    menuButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(menuButton, &QPushButton::clicked, this, &Informations::backToMenu);//if clicked, go back to menu page

    exitAppButton = new QPushButton("Exit", this);//exit app
    exitAppButton->setFont(font);
    exitAppButton->setGeometry(x_pos, y_pos+y_siz+gap, x_size, y_siz);
    exitAppButton->setStyleSheet("background-color: rgba(255, 253, 208, 153);color: black;");//transparency is equal to 153/255 ->abous 60%
    connect(exitAppButton, &QPushButton::clicked, this, &Informations::exitApp);

}

void Informations::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);//Background
    QWidget::paintEvent(event);
}

void Informations::exitApp()
{
    this->close();//close this window
}

void Informations::backToMenu()//go back to main menu
{
    MenuPage *menPage = new MenuPage();
    menPage->show();//display menu page
    this->close();//close this page
}

void Informations::startSimul()
{
    MainWindow *simPage = new MainWindow();
    simPage->show();//display simulation page
    this->close();//close this page
}

