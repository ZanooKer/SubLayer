#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNumLayers(int num)
{
    this->numberOfLayer = num;
}

void MainWindow::setStartLayers(int num)
{
    this->startNumLayer = num;
}

void MainWindow::rotateOneStep()
{
    glWidgets->rotateBy(0, +2 * 16, 0);
}

void MainWindow::enableResize(int w,int h)
{
    reWidth = w;
    reHeight = h;
    resize = true;
}

void MainWindow::visualize()
{
    QColor clearColor;
    clearColor.setHsv(0,255, 63);
    glWidgets = new GLWidget();
    glWidgets ->setClearColor(clearColor);
    glWidgets ->rotateBy(+42 * 16, +42 * 16, 0);
    glWidgets->setCurrentPP(pps);
    ui->layout->addWidget(glWidgets);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::rotateOneStep);
    timer->start(20);
}

void MainWindow::addImage(QString filename,int layer)
{
    QImage image(filename);
    /*if(resize)
    {
        image = resizeResolution(image);
    }*/
    PicturePiece pp(image,layer);
    pps.push_back(pp);
}

QImage resizeResolution(QImage in)
{
    //set
}
