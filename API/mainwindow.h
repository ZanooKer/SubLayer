#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "picturepiece.h"
#include "glwidget.h"
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void visualize();
    void setNumLayers(int num);
    void setStartLayers(int num);
    void addImage(QString filename,int layer);
//    void enableResize(int width,int height);
    QImage resizeResolution(QImage in);
    float real2Scale;
    int maxWidth;
    int maxHeight;
    void setLayerDiff(std::map<int,int> diff);
    std::vector<PicturePiece> getPPS();
    std::map<int,float> getLayerDiff();

private slots:
    void rotateOneStep();

private:
    Ui::MainWindow *ui;
    int numberOfLayer;
    std::map<int,float> layerDiff;
    std::vector<PicturePiece> pps;
    GLWidget *glWidgets;
};

//get mode of API : visualize or export
//set number of image
//set image file
//set based pixels of layers 0 (reference to other layer) : get in centimeter to pixels
//set different between each layers in centimeter

#endif // MAINWINDOW_H
