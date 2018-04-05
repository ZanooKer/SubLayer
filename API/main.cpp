#include "mainwindow.h"
#include <QApplication>
#include <iostream>

QString findBasedPath(QString textfile)
{
    printf("find ratio from based layer (layer 0)\n");
    QFile file(textfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QString basedFile = " ";
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        QString l = fields.at(0);
        if(l.toInt() == 0)
        {
            printf("found %s as based layer\n", fields.at(1).toStdString().c_str());
            basedFile = fields.at(1);
            break;
        }
    }
    file.close();
    return basedFile;
}

std::vector<int> findRatioAndProp(QString filename,int width,int height)
{
    QString basedFile = findBasedPath(filename);

    std::vector<int> ans;
    if(basedFile == " ")
    {
        ans.push_back(0);
        return ans;
    }
    else
    {
        QImage image(basedFile);
        printf("Set each layer have maximum width : %d \n",image.width());
        printf("Set each layer have maximum height : %d \n",image.height());
        float sc = float(image.width())/width;
        float sct = float(image.height())/height;
        int scal = (int)std::max(sc,sct);
        ans.push_back(scal*width);
        ans.push_back(scal*height);
        ans.push_back(scal);
        return ans;
    }
}

void addAllImages(MainWindow &w,QString textfile)
{
    printf("Loading image to windows\n");
    QFile file(textfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QString basedFile = " ";
    int line = 0;
    std::vector<int> nextDiff;
    std::map<int,int> layerDiff;
    int numLayer;
    while(!in.atEnd()) {
        QString lineText = in.readLine();
        QStringList fields = lineText.split(",");
        if(line == 0)
        {
            QString temp = fields.at(0);
            numLayer = temp.toInt();
            for(int i = 1;i<numLayer;i++)
            {
                temp = fields.at(i);
                nextDiff.push_back(temp.toInt());
            }
        }
        else {
            QString l = fields.at(0);
            QString temp = fields.at(1);
            if(line < nextDiff.size()+1)
            {
                int n = l.toInt();
                layerDiff[n] = nextDiff[line-1];
                printf("Layer %d to %d have %d millimeter\n",n,n+1,layerDiff[n]);
            }
            w.addImage(temp,l.toInt());
        }
        line++;
    }
    file.close();

    w.nextDiff = layerDiff;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //get mode of API : visualize or export
    //set number of image
    //set image file
    //set based pixels of layers 0 (reference to other layer) : get in centimeter to pixels
    //set different between each layers in millimeter
    //./API visualize 10 10 --auto-scale source.txt
    //   0     1       2 3    4           5

    QString cmd = " ";
    int numberOfLayers = 0;
    if(argc != 6)
    {
        printf("%d\n",argc);
        printf("You don't write in format as description. Please --help to see all configs.");
    }
    else
    {
        cmd = QString(argv[1]);
        QString filename = QString(argv[5]);
        if(cmd == QString("visualize"))
        {
            int realWidth = atoi(argv[2]);
            int realHeight = atoi(argv[3]);
            int pixPerReal,maxWidth,maxHeight;
            if(QString(argv[4]) == QString("--auto-scale"))
            {
                std::vector<int> prop = findRatioAndProp(filename,realWidth,realHeight);
                maxWidth = prop[0];
                maxHeight = prop[1];
                pixPerReal = prop[2];
            }
            else
            {
                QString basedPath = findBasedPath(filename);
                QImage im(basedPath);
                pixPerReal = atoi(argv[4]);
                maxWidth = std::min(pixPerReal * realWidth,im.width());
                maxHeight = std::min(pixPerReal * realHeight,im.height());
                printf("Set each layer have maximum width : %d \n",maxWidth);
                printf("Set each layer have maximum height : %d \n",maxHeight);
            }
            w.real2Scale = pixPerReal;
            w.maxWidth = maxWidth;
            w.maxHeight = maxHeight;
            printf("Set %d pixels per one millimeter.\n", pixPerReal);

            addAllImages(w,filename);
            w.visualize();
            w.show();
            return a.exec();
        }

    }
    //return a.exec();
}