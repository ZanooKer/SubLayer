#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    editorscene =(Editor*) (new QGraphicsScene(this));
    ui->graphicsView->setScene(editorscene);

    layerscene = (LayerEditor*)new QGraphicsScene(this);
    ui->graphicsView_2->setScene(layerscene);

    ui->lineEdit->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,tr("Open file"),"C://","PNG files(*.png);;All files(*.*)"
                );

    ui->lineEdit->setText(filename);

    QImage image(filename);
    QRect qr = ui->graphicsView->geometry();
    QSize A(qr.width(),qr.height());
    image = image.scaled(A,Qt::KeepAspectRatio);
    PicturePiece pp(image,0);
    pps.push_back(pp);

    //editorscene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(image2)));
    editorscene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(pps.back().gi)));
    reloadScene();

    QMessageBox::information(this,tr("Loading File"),"load "+filename+" successfully");
}

void MainWindow::reloadScene()
{
    layerscene->addItemFromLayer(pps,ui->spinBox->value());
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    reloadScene();
}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::resizePicture()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    GLWindows *glwin = new GLWindows();
    glwin->glWidgets->setCurrentPP(pps);
    glwin->show();
}
