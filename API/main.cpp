#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "pixelbox.h"

template <typename T>
static void
_WriteElement(std::ostream &os, T element)
{
    os.write(reinterpret_cast<const char*>(&element), sizeof(T));
}

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

std::vector<int> fromStartToDest(std::map<int,int> eachDiff)
{
    std::vector<int> temp;
    if(eachDiff.find(0) == eachDiff.end())
    {
        return temp;
    }
    temp.push_back(0);
    int idx = 1;
    temp.push_back(temp.back() + eachDiff[0]);
    while(idx < eachDiff.size())
    {
        temp.push_back(temp.back()+eachDiff[idx]);
        idx++;
    }
    return temp;

}

void addImageToGrid(PixelBox &pb,QString textfile)
{
    QFile file(textfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    int line = 0;
    std::vector<int> nextDiff;
    std::map<int,int> layerDiff;
    std::list<int> allLayers;
    std::map<int,QImage> imageL;
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
            printf("layer #%d : %s",l.toInt(),temp.toStdString().c_str());
            imageL[l.toInt()] = QImage(temp);
            allLayers.push_back(l.toInt());
            if(line < nextDiff.size()+1)
            {
                int n = l.toInt();
                layerDiff[n] = nextDiff[line-1];
                printf(" -> below layer #%d : %d millimeter\n",n+1,layerDiff[n]);
            }
        }
        line++;
    }
    file.close();
    if(!checkInvalidLayer(layerDiff,allLayers))
    {
        nextDiff.clear();
        return;
    }

    std::vector<int> gridHeight = fromStartToDest(layerDiff);
    if(gridHeight.size() == imageL.size())
    {
        printf("AAAa");
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //./API --help
    //./API visualize 10 10 --auto-scale source.txt
    //./API exportO3DP 10 10 --auto-scale source.txt out.o3dp

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

    else if(argc != 6 && argc != 7)
    {
        printf("%d",argc);
        printf("You don't write in format as description. Please --help to see all configs.\n");
    }
    else
    {
        cmd = QString(argv[1]);
        int realWidth = atoi(argv[2]);
        int realHeight = atoi(argv[3]);
        QString modeOfRes = QString(argv[4]);
        QString filename = QString(argv[5]);

        int pixPerReal,maxWidth,maxHeight;
        if(modeOfRes == QString("--auto-scale"))
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
        printf("Set %d pixels per one millimeter.\n", pixPerReal);

        if(cmd == QString("visualize"))
        {
            printf("\n");
            w.real2Scale = pixPerReal;
            w.maxWidth = maxWidth;
            w.maxHeight = maxHeight;
            bool addImg = addAllImages(w,filename);
            if(!addImg)return 0;

            printf("\n");
            printf("visualize...\n");
            w.visualize();
            w.show();
            return a.exec();
        }
        else if(cmd == QString("exportO3DP"))
        {
            //std::vector<int> gridHeight = getAllReal(filename);
            PixelBox pb;
            addImageToGrid(pb,filename);
            //format material is 255*255*256 if that pixel equals to 0 means transparent(alpha 0)


        }
    }
    //return a.exec();
}
