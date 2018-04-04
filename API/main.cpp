#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <iostream>

int findRatio(QString filename,int width,int height)
{
    printf("find ratio from based layer (layer 0)\n");
    QFile file(filename);
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

    if(basedFile == " ")
    {
        return 0;
    }
    else
    {
        QImage image(basedFile);
        printf("based layer's file have width : %d \n",image.width());
        printf("based layer's file have height : %d \n",image.height());
        float sc = float(image.width())/width;
        float sct = float(image.height())/height;
        return (int)std::max(sc,sct);
    }
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
            int pixPerReal;
            if(QString(argv[4]) == QString("--auto-scale"))
            {
                pixPerReal = findRatio(filename,realWidth,realHeight);
            }
            else
            {
                pixPerReal = atoi(argv[4]);
            }
            w.real2Scale = pixPerReal;
            printf("Set %d pixels per one millimeter.\n", pixPerReal);

            /*printf("Your object have %d layers \n",numberOfLayers);
            w.setNumLayers(numberOfLayers);
            char* moderator = argv[3];
            if(QString(moderator) == QString("-r"))
            {
                if(argc > 3)
                {
                    printf("set based layer to have maximum pixels as 50x50");
                    w.enableResize(50,50);
                }
            }

            for(int i=0;i<numberOfLayers;i++)
            {
                printf("Loading: /home/zanooker/Arrow.png as layer %d \n", i);
                w.addImage("/home/zanooker/Arrow.png",i);
            }
            w.visualize();
            w.show();
            return a.exec();*/
        }

    }
    //return a.exec();
}
