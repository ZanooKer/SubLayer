#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "pixelbox.h"

struct dataset{
    int numLayer;
    std::map<int,int> nextReal;
    std::map<int,QString> relate;
};

template <typename T>
static void
_WriteElement(std::ostream &os, T element)
{
    os.write(reinterpret_cast<const char*>(&element), sizeof(T));
}

dataset prepareFromTxt(QString textfile)
{
    QFile file(textfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    dataset d;
    std::vector<int> next;
    int lines = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        QString l = fields.at(0);
        if(lines == 0)
        {
            d.numLayer = l.toInt();
            for(int i=1;i<d.numLayer;i++)
            {
                QString s = fields.at(i);
                next.push_back(s.toInt());
            }
        }
        else
        {
            QString nL = fields.at(0);
            QString path = fields.at(1);
            if(lines-1 < (int) next.size())
                d.nextReal[nL.toInt()] = next[lines-1];
            d.relate[nL.toInt()] = path;
        }
        lines++;
    }
    file.close();
    return d;
}

int findRatio(QImage im,int rw,int rh)
{
   return std::max(1,std::max(im.width()/rw,im.height()/rh));
}

bool checkInvalidLayer(std::map<int,int> diff,std::map<int,QString> alls)
{
    bool ans = false;
    for(std::pair<int,int> e:diff)
    {
        printf("check for layer %d and %d...",e.first,e.first+1);
        bool firstInVect = false;
        bool lastInVect = false;
        for(std::pair<int,QString> i:alls)
        {
            if(firstInVect && lastInVect)
                break;
            if(i.first == e.first && !firstInVect)
            {
                firstInVect = true;
            }
            if(i.first == e.first+1 && !lastInVect)
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

void addAllImages(MainWindow &w,std::map<int,QString> pathL)
{
    printf("Loading image to windows\n");
    for(std::pair<int,QString> p:pathL)
    {
        w.addImage(p.second,p.first);
    }
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

        //Prepare data from text file
        //number of layer
        //millimeter to next layer
        //#layer and image
        printf("prepare dataset\n");
        dataset raw = prepareFromTxt(filename);

        //check that next layer is valid or not
        printf("\ncheck for invalid layers\n");
        if(!checkInvalidLayer(raw.nextReal,raw.relate))
            return 0;

        //check that it have based layer
        if(raw.relate.find(0) == raw.relate.end()){
            printf("Cannot find based layer\n");
            return 0;
        }

        printf("\n");
        //find pixel per millimeter
        int pixPerReal,maxWidth,maxHeight;
        if(modeOfRes == QString("--auto-scale"))
        {
            printf("Set auto scale based on layer's images : %s \n",raw.relate[0].toStdString().c_str());
            QImage im(raw.relate[0]);
            maxWidth = im.width();
            maxHeight = im.height();
            pixPerReal = findRatio(im,realWidth,realHeight);
        }
        else
        {
            pixPerReal = modeOfRes.toInt();
            maxWidth = pixPerReal*realWidth;
            maxHeight = pixPerReal*realHeight;
        }
        printf("Set each layer have maximum width : %d \n",maxWidth);
        printf("Set each layer have maximum height : %d \n",maxHeight);
        printf("Set %d pixels per one millimeter.\n", pixPerReal);

        printf("\n");
        if(cmd == QString("visualize"))
        {
            printf("\n");
            w.real2Scale = pixPerReal;
            w.maxWidth = maxWidth;
            w.maxHeight = maxHeight;
            w.setLayerDiff(raw.nextReal);
            addAllImages(w,raw.relate);

            printf("\n");
            printf("visualize...\n");
            w.visualize();
            w.show();
            return a.exec();
        }
        else if(cmd == QString("exportO3DP"))
        {
            PixelBox pb;


            std::vector<int> allHeight = fromStartToDest(raw.nextReal);
            printf("Input Grid : %d %d %d\n",realWidth,realHeight,allHeight.back());
            int ratio = int(MM_TO_INCH * DPI); //from mm_to_inch and Default DPI (300-500)
            int outGridW = realWidth*ratio;
            int outGridL = realHeight*ratio;
            int outGridH = allHeight.back()*ratio;
            printf("Output Grid : %d %d %d \n",outGridW,outGridL,outGridH);
            pb.allocatePlane(outGridW,outGridL,outGridH);

            for(int i=0;i<allHeight.size();i++){
                QImage img(raw.relate[i]);
                pb.addImageToPlane(img,allHeight[i]);
            }
            QString out = QString(argv[6]);
            pb.writeO3DP(out,outGridW,outGridL,outGridH);
        }
    }
}
