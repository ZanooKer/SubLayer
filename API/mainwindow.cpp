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

void MainWindow::setLayerDiff(std::map<int,int> diff)
{
    std::map<int,float> temp;
    temp[0] = 0;
    float grid = std::max(maxHeight,maxWidth);
    for(std::pair<int,int> d:diff)
    {
        temp[d.first+1] = temp[d.first] + diff[d.first]*real2Scale/grid;
    }
    this->layerDiff = temp;
}

std::vector<PicturePiece> MainWindow::getPPS()
{
    return this->pps;
}

std::map<int,float> MainWindow::getLayerDiff()
{
    return this->layerDiff;
}

void MainWindow::visualize()
{
    QColor clearColor;
    clearColor.setHsv(0,255, 63);
    glWidgets = new GLWidget();
    glWidgets ->setClearColor(clearColor);
    glWidgets ->rotateBy(+42 * 16, +42 * 16, 0);
    glWidgets->setMaxSizePic(maxWidth,maxHeight);
    glWidgets->setDiffHeight(layerDiff);
    glWidgets->setCurrentPP(pps);
    ui->layout->addWidget(glWidgets);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::rotateOneStep);
    timer->start(20);
}

void MainWindow::addImage(QString filename,int layer)
{
    QImage image(filename);
    printf("Add %s as layer %d \n",filename.toStdString().c_str(),layer);
    if(image.width() > maxWidth && image.height() >  maxHeight)
    {
        image = resizeResolution(image);
    }
    PicturePiece pp(image,layer);
    pps.push_back(pp);
}

QImage MainWindow::resizeResolution(QImage in)
{
    QImage ans = in.scaled(QSize(maxWidth,maxHeight),Qt::KeepAspectRatio);
    printf("Decrease resolution from %d , %d to %d , %d \n", in.width(),in.height(),ans.width(),ans.height());
    return ans;
}
