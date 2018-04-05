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

bool checkInvalidLayer(std::map<int,int> diff,std::list<int> layers)
{
    printf("\nchecking for invalid layers... \n");
    bool ans = false;
    for(std::pair<int,int> e:diff)
    {
        printf("check for layer %d and %d...",e.first,e.first+1);
        bool firstInVect = false;
        bool lastInVect = false;
        for(int i:layers)
        {
            if(firstInVect && lastInVect)
                break;
            if(i == e.first && !firstInVect)
            {
                firstInVect = true;
            }
            if(i == e.first+1 && !lastInVect)
            {
                lastInVect = true;
            }
        }
        if(!(firstInVect && lastInVect))
        {
            ans = true;
            printf("not found\n");
            break;
        }
        printf("found\n");
    }
    return !ans;
}

bool addAllImages(MainWindow &w,QString textfile)
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
    std::list<int> allLayers;
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
            w.addImage(temp,l.toInt());
            allLayers.push_back(l.toInt());
            if(line < nextDiff.size()+1)
            {
                int n = l.toInt();
                layerDiff[n] = nextDiff[line-1];
                printf("Layer %d to %d have %d millimeter\n",n,n+1,layerDiff[n]);
            }
        }
        line++;
    }
    file.close();
    w.setLayerDiff(layerDiff);
    return checkInvalidLayer(layerDiff,allLayers);
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
    if(QString(argv[1]) == QString("--help"))
    {
        printf("Format: ./API [mode of API] [millimeter in width] [millimeter in height] [mode of resolution] [text-file path]\n");
        printf("        [mode of API] consists of \'visualize\' and \'exportO3DP\' not done yet \n");
        printf("        [millimeter in width] is integer value\n");
        printf("        [millimeter in height] is integer value\n");
        printf("        [mode of resolution] is integer value equals to pixels per millimeter or \'--auto-scale\' if you want to auto scale pixels per millimeter based on picture in layer 0\n");
        printf("        [text-file path] is the path to text file which is described below. \n\n");

        printf("text file describes number of layers, millimeter between each layers, layer number and image paths related to layer number\n");
        printf("Example of txt files is \n");
        printf("----------------------------------------------------\n");
        printf("|#layers,d0,d1,d2,...\t| \n"
               "|l0,image0.png\t\t|\n"
               "|l1,image1.png\t\t|\n"
               "|.\t\t\t|\n"
               "|.\t\t\t|\n"
               "|.\t\t\t|\n");
        printf("----------------------------------------------------\n");
        printf("Suggestions:\n1. d0 is the number of millimeter between l0 and l0 plus one. NOT! l0 and l1\n");
        printf("2. l0,l1 and so on are not necessary to be consecutive.But dn are related to ln. \n");
    }

    else if(argc != 6)
    {
        printf("You don't write in format as description. Please --help to see all configs.\n");
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
                pixPerReal = atoi(argv[4]);
                maxWidth = pixPerReal*realWidth;
                maxHeight = pixPerReal*realHeight;
                printf("Set each layer have maximum width : %d \n",maxWidth);
                printf("Set each layer have maximum height : %d \n",maxHeight);
            }
            w.real2Scale = pixPerReal;
            w.maxWidth = maxWidth;
            w.maxHeight = maxHeight;
            printf("Set %d pixels per one millimeter.\n", pixPerReal);

            printf("\n");
            if(addAllImages(w,filename)){
                printf("\n");
                printf("visualize...\n");
                w.visualize();
                w.show();
                return a.exec();
            }
        }

    }
    //return a.exec();
}
