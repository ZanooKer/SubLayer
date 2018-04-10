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

dataset prepareIndiv(QString textfile)
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
        printf("%s\n",line.toStdString().c_str());
        QStringList fields = line.split(",");
        QString l = fields.at(0);
        if(lines == 1)
        {
            d.numLayer = l.toInt();
            printf("%d\n",d.numLayer);
            for(int i=1;i<d.numLayer;i++)
            {
                QString s = fields.at(i);
                next.push_back(s.toInt());
            }
        }
        else if(lines > 1)
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

dataset prepareTerrain(QString textfile,int widthProduct,int heightProduct)
{
    QFile file(textfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    std::map<int,QString> layNumToImg;
    std::vector<int> eachLayHeight;;

    QTextStream in(&file);
    QImage imageBased;
    dataset d;
    int widthPict,heightPict;
    int lines = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if(lines==1) //read header
        {
            d.numLayer = fields.at(0).toInt();
            layNumToImg[0] = QString(fields.at(1));
            imageBased = QImage(QString(fields.at(1)));
        }
        else if(lines == 2)
        {
            widthPict = fields.at(0).toInt();
            heightPict = fields.at(1).toInt();
        }
        else if(lines > 2)
        {
            eachLayHeight.push_back(fields.at(0).toInt());
            int rf,gf,bf;
            rf = fields.at(1).toInt();
            gf = fields.at(2).toInt();
            bf = fields.at(3).toInt();
            QImage temp = imageBased;
            for(int i=0;i<imageBased.height();i++)
            {
                for(int j=0;j<imageBased.width();j++)
                {
                    QColor pixel = imageBased.pixelColor(j,i);
                    QColor p(0,0,0,0);
                    if(std::abs(pixel.red()-rf) < 10 &&
                            std::abs(pixel.green()-gf) < 10 &&
                            std::abs(pixel.blue()-bf) < 10){
                        p = pixel;
                    }
                    temp.setPixelColor(j,i,p);
                }
            }
            char a[50];
            std::sprintf(a,"temp%d.png",lines-2);
            QString pathOut(a);
            QImageWriter iw(pathOut);
            iw.setOptimizedWrite( true );
            iw.setProgressiveScanWrite( true );
            iw.write(temp);
            layNumToImg[fields.at(0).toInt()] = pathOut;
        }
        lines++;
    }
    int nl = 0;
    int currentH = 0;
    d.relate[0] = layNumToImg[0];
    float scale = std::max(widthProduct*1.00f/widthPict,heightProduct*1.00f/heightPict);
    for (auto it=++layNumToImg.begin(); it!=layNumToImg.end(); ++it)
    {
        d.nextReal[nl] = (int)std::ceil((it->first - currentH)*scale);
        currentH = it->first;
        nl++;
        printf("write file: %s as heights %d -> layer : %d \n",it->second.toStdString().c_str(),it->first,nl);
        d.relate[nl] = it->second;
    }
    return d;
}

dataset prepareFromText(QString textfile,int widthProduct,int heightProduct)
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
        if(lines == 0)
        {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            QString l = fields.at(0);
            if(l == QString("Single"))
            {
                printf("A");
                return prepareIndiv(textfile);
            }
            else if(l == QString("Map"))
            {
                return prepareTerrain(textfile,widthProduct,heightProduct);
            }
        }
    }
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

    QString cmd = " ";
    if(argc == 2 && QString(argv[1]) == QString("--help"))
    {
        std::ifstream inFile;
        inFile.open("help.txt");
        if( inFile.is_open() )
        {
          std::string s;
          while( std::getline( inFile, s ) )
          {
            std::cout << s << std::endl;
          }
          inFile.close();
        }
        else
          std::cout << "Error opening file " << errno << std::endl;
    }
    else if(
            (argc == 2 && (QString(argv[1])==QString("visualize-single") || QString(argv[1]) == QString("exportO3DP-single")))
             || (argc == 2 && (QString(argv[1])==QString("visualize-map") || QString(argv[1]) == QString("exportO3DP-map")))
             || (argc == 6 && (QString(argv[1])==QString("visualize")))
             || (argc == 7 && (QString(argv[1])==QString("exportO3DP")))
           )
    {
        int realWidth,realHeight;
        QString modeOfRes,filename;
        if(argc == 2 && (QString(argv[1])==QString("visualize-single") || QString(argv[1]) == QString("exportO3DP-single")))
        {
            modeOfRes = QString("--auto-scale");
            printf("Your width of product(millimeter) :");
            scanf("%d",&realWidth);
            printf("Your height of product(millimeter) :");
            scanf("%d",&realHeight);

            int numL;
            std::map<int,QString> path;
            std::vector<int> nextf;
            char fil[80];
            printf("Number of layer of your file :");
            scanf("%d",&numL);
            for(int i=0;i<numL;i++){
                printf("Layer %d -- ",i);
                printf("set image path :");
                scanf("%79s",fil);
                path[i] = QString(fil);
                printf("height from previous layer(mm) : ");
                if(i==0){
                    printf("0\n");
                }else{
                    int temp;
                    scanf("%d",&temp);
                    nextf.push_back(temp);
                }
            }
            cmd = (QString(argv[1])==QString("visualize-single"))?"visualize":"exportO3DP";

            //write text file config
            std::ofstream infile;
            std::string p = "temp_config.txt";
            filename = QString(p.c_str());
            infile.open(p);
            //Header
            infile << "Single\n";
            infile << numL;
            for(int i=0;i<nextf.size();i++){
                infile << "," << nextf[i];
            }
            infile << "\n";
            //each layer
            for(int i=0;i<numL;i++){
                infile << i << "," << path[i].toStdString() << "\n";
            }
            infile.close();
        }
        else if(argc == 2 && (QString(argv[1])==QString("visualize-map") || QString(argv[1]) == QString("exportO3DP-map")))
        {
            modeOfRes = QString("--auto-scale");
            cmd = (QString(argv[1])==QString("visualize-map"))?"visualize":"exportO3DP";
            printf("Your width of product(millimeter) :");
            scanf("%d",&realWidth);
            printf("Your height of product(millimeter) :");
            scanf("%d",&realHeight);

            int numL;
            char fil[80];
            std::ofstream infile;
            std::string p = "temp_config.txt";
            filename = QString(p.c_str());
            infile.open(p);
            infile << "Map\n";
            printf("Number of layer of your file :");
            scanf("%d",&numL);
            infile << numL << ",";
            printf("Set Image path of map : ");
            scanf("%79s",fil);
            infile << std::string(fil) << "\n";
            printf("set real scale of image axisX (should be same scale as height and deep): ");
            int temp;
            scanf("%d",&temp);
            infile << temp << ",";
            printf("set real scale of image axisY: ");
            scanf("%d",&temp);
            infile << temp << "\n";
            for(int i=1;i<numL;i++){
                printf("Layer %d -- ",i);
                printf("set real scale of this layer's height : ");
                scanf("%d",&temp);
                infile << temp << ",";
                printf("set RGB color : ");
                for(int j=0;j<3;j++){
                    scanf("%d",&temp);
                    infile << "," << temp;
                }
                infile << "\n";
            }
            infile.close();
        }
        else{
            cmd = QString(argv[1]);
            realWidth = atoi(argv[2]);
            realHeight = atoi(argv[3]);
            modeOfRes = QString(argv[4]);
            filename = QString(argv[5]);
        }

        //Prepare data from text file
        printf("prepare dataset\n");
        dataset raw = prepareFromText(filename,realWidth,realHeight);

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
            pb.allocatePlane(realWidth,realHeight,allHeight.back());

            printf("%d",allHeight.size());
            for(int i=0;i<allHeight.size();i++){
                QImage img(raw.relate[i]);
                pb.addImageToPlane(img,allHeight[i]);
            }

            int ratio = int(MM_TO_INCH * DPI); //from mm_to_inch and Default DPI (300-500)
            int outGridW = realWidth*ratio;
            int outGridL = realHeight*ratio;
            int outGridH = allHeight.back()*ratio;
            printf("Output Grid : %d %d %d \n",outGridW,outGridL,outGridH);

            QString out = QString(argv[6]);
            pb.writeO3DP(out,outGridW,outGridL,outGridH);
        }
    }
    else{
        printf("You don't write in format as description. Please --help to see all configs.\n");
    }
}
