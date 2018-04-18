#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "pixelbox.h"
#include <vector>
#include <sstream>

#define gridXY_mm 0.035
#define gridZ_mm 0.0125

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

bool has_only_digits(const std::string s){
  return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

dataset prepareIndiv(QString textfile)
{
    QFile file(textfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    dataset d;
    dataset d_fail;
    d_fail.numLayer = -1;
    std::vector<int> next;
    int lines = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        QString l = fields.at(0);
        if(lines == 1)
        {
            if(!has_only_digits(l.toStdString())){
                printf("Total count of layer isn't integer\n");
                return d_fail;
            }
            d.numLayer = l.toInt();
            for(int i=1;i<d.numLayer;i++)
            {
                QString s = fields.at(i);
                if(!has_only_digits(s.toStdString())){
                    printf("Some distance between each layers isn't integer\n");
                    return d_fail;
                }
                next.push_back(s.toInt());
            }
        }
        else if(lines > 1)
        {
            QString nL = fields.at(0);
            QString path = fields.at(1);
            if(!has_only_digits(nL.toStdString())){
                printf("Some layer's number isn't integer\n");
                return d_fail;
            }
            if(lines-1 <= (int) next.size()){
                d.nextReal[nL.toInt()] = next[lines-2];
            }
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
    dataset d_fail;
    d_fail.numLayer = -1;
    int widthPict,heightPict;
    int lines = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if(lines==1) //read header
        {
            if(!has_only_digits(fields.at(0).toStdString())){
                printf("Total count of layer isn't integer\n");
                return d_fail;
            }
            d.numLayer = fields.at(0).toInt();
            layNumToImg[0] = QString(fields.at(1));
            imageBased = QImage(QString(fields.at(1)));
        }
        else if(lines == 2)
        {
            if(!has_only_digits(fields.at(0).toStdString())){
                printf("Real width of based picture isn't integer\n");
                return d_fail;
            }
            widthPict = fields.at(0).toInt();
            if(!has_only_digits(fields.at(1).toStdString())){
                printf("Real height of based picture isn't integer\n");
                return d_fail;
            }
            heightPict = fields.at(1).toInt();
        }
        else if(lines > 2)
        {
            if(!has_only_digits(fields.at(0).toStdString())){
                printf("Some layer's height isn't integer\n");
                return d_fail;
            }
            eachLayHeight.push_back(fields.at(0).toInt());
            int rf,gf,bf;
            if(!has_only_digits(fields.at(1).toStdString())){
                printf("Some red value of pixel isn't integer\n");
                return d_fail;
            }
            rf = fields.at(1).toInt();
            if(!has_only_digits(fields.at(2).toStdString())){
                printf("Some green value of pixel isn't integer\n");
                return d_fail;
            }
            gf = fields.at(2).toInt();
            if(!has_only_digits(fields.at(3).toStdString())){
                printf("Some blue value of pixel isn't integer\n");
                return d_fail;
            }
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
    int lines = 0;
    dataset d;
    d.numLayer = -1;
    while(!in.atEnd()) {
        if(lines == 0)
        {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            QString l = fields.at(0);
            if(l == QString("Single"))
            {
                return prepareIndiv(textfile);
            }
            else if(l == QString("Map"))
            {
                return prepareTerrain(textfile,widthProduct,heightProduct);
            }
            else {
                printf("Could not find format type in header\n.");
                return d;
            }
        }
    }
    file.close();
    printf("Input file isn't found\n.");
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

int fromStartToDest(std::map<int,int> eachDiff)
{
    if(eachDiff.size() == 0){
        return 1;
    }
    int count_layer = 1;
    foreach(auto n,eachDiff){
        count_layer+=n.second;
    }
    return count_layer;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString cmd = " ";
    if(argc == 2 && QString(argv[1]) == QString("--help"))
    {
        //show help file
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
        QString outfile;
        if(argc == 2 && (QString(argv[1])==QString("visualize-single") || QString(argv[1]) == QString("exportO3DP-single")))
        {
            std::string temp_in;
            //get input from user
            modeOfRes = QString("--auto-scale");
            printf("Your width of product(millimeter) :");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            realWidth = atoi(temp_in.c_str());
            temp_in.clear();
            printf("Your height of product(millimeter) :");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            realHeight = atoi(temp_in.c_str());
            temp_in.clear();

            int numL;
            std::map<int,QString> path;
            std::vector<int> nextf;
            printf("Number of layer of your file :");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            numL = atoi(temp_in.c_str());
            temp_in.clear();
            for(int i=0;i<numL;i++){
                printf("Layer %d -- ",i);
                printf("set image path :");
                std::getline (std::cin,temp_in);
                path[i] = QString(temp_in.c_str());
                temp_in.clear();
                printf("height from previous layer(mm) : ");
                if(i==0){
                    printf("0\n");
                }else{
                    std::getline(std::cin,temp_in);
                    if(!has_only_digits(temp_in)){
                        printf("Terminated\n");
                        return 0;
                    }
                    nextf.push_back(atoi(temp_in.c_str()));
                    temp_in.clear();
                }
            }
            cmd = (QString(argv[1])==QString("visualize-single"))?"visualize":"exportO3DP";
            if(cmd == "exportO3DP"){
                printf("name of your output file : ");
                std::getline (std::cin,temp_in);
                outfile = QString(temp_in.c_str());
                temp_in.clear();
            }

            //write text file config
            std::ofstream infile;
            std::string p = "temp_config.txt";
            filename = QString(p.c_str());
            infile.open(p);
            //1.Write Header
            infile << "Single\n";
            infile << numL;
            for(int i=0;i<nextf.size();i++){
                infile << "," << nextf[i];
            }
            infile << "\n";
            //2.Write each layer
            for(int i=0;i<numL;i++){
                infile << i << "," << path[i].toStdString() << "\n";
            }
            infile.close();
        }
        else if(argc == 2 && (QString(argv[1])==QString("visualize-map") || QString(argv[1]) == QString("exportO3DP-map")))
        {
            //get input from user
            std::string temp_in;
            modeOfRes = QString("--auto-scale");
            cmd = (QString(argv[1])==QString("visualize-map"))?"visualize":"exportO3DP";
            printf("Your width of product(millimeter) :");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            realWidth = atoi(temp_in.c_str());
            temp_in.clear();
            printf("Your height of product(millimeter) :");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            realHeight = atoi(temp_in.c_str());
            temp_in.clear();

            //write text file with input from user
            //becaused the sequence of inputs are consecutive same as text file.
            int numL;
            std::ofstream infile;
            std::string p = "temp_config.txt";
            filename = QString(p.c_str());
            infile.open(p);
            infile << "Map\n";
            printf("Number of layer of your file :");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            numL = atoi(temp_in.c_str());
            temp_in.clear();
            infile << numL << ",";

            printf("Set Image path of map : ");
            std::getline (std::cin,temp_in);
            infile << temp_in << "\n";
            temp_in.clear();

            printf("set real scale of image axisX (should be same scale as height and deep): ");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            infile << temp_in << ",";
            temp_in.clear();

            printf("set real scale of image axisY: ");
            std::getline (std::cin,temp_in);
            if(!has_only_digits(temp_in)){
                printf("Terminated\n");
                return 0;
            }
            infile << temp_in << "\n";
            temp_in.clear();

            for(int i=1;i<numL;i++){
                printf("Layer %d -- ",i);
                printf("set real scale of this layer's height : ");
                std::getline (std::cin,temp_in);
                if(!has_only_digits(temp_in)){
                    printf("Terminated\n");
                    return 0;
                }
                infile << temp_in;
                temp_in.clear();
                printf("set RGB color : ");
                std::getline (std::cin,temp_in);
                std::stringstream lineStream;
                lineStream << temp_in;
                std::string token;
                int count = 0;
                while(lineStream >> token && count < 3)
                {
                    if(!has_only_digits(token)){
                        printf("Terminated\n");
                        return 0;

                    }
                    infile << "," << token;
                    count++;
                }
                temp_in.clear();


//                for(int j=0;j<3;j++){
//                    std::getline (std::cin,temp_in);
//                    if(!has_only_digits(temp_in)){
//                        printf("Terminated\n");
//                        return 0;
//                    }
//                    infile << temp_in << ",";
//                    temp_in.clear();
//                }
                infile << "\n";
            }
            infile.close();
            if(cmd == "exportO3DP"){
                printf("name of your output file : ");
                std::getline (std::cin,temp_in);
                outfile = QString(temp_in.c_str());
                temp_in.clear();
            }
        }
        else{
            //all input are from command
            cmd = QString(argv[1]);
            std::string temp = QString(argv[2]).toStdString();
            if(has_only_digits(temp)){
                realWidth = atoi(argv[2]);
            }
            else{
                printf("Real width isn't integer number\nTerminated\n");
                return 0;
            }
            temp = QString(argv[3]).toStdString();
            if(has_only_digits(temp)){
                realHeight = atoi(argv[3]);
            }
            else{
                printf("Real height isn't integer number\nTerminated\n");
                return 0;
            }
            modeOfRes = QString(argv[4]);
            filename = QString(argv[5]);
            if(cmd == "exportO3DP")
                outfile = QString(argv[6]);
        }




        //Prepare data from text file
        printf("\nprepare dataset from text file\n");
        dataset raw = prepareFromText(filename,realWidth,realHeight);
        if(raw.numLayer == -1){
            printf("load dataset failed\nTerminated\n");
            return 0;
        }
        printf("load dataset successfully\n");



        //check that next layer is valid or not
        printf("\ncheck for invalid layers\n");
        if(!checkInvalidLayer(raw.nextReal,raw.relate)){
            printf("found invalid layer\nTerminated\n");
            return 0;
        }
        //check that it have based layer
        if(raw.relate.find(0) == raw.relate.end()){
            printf("Cannot find layer #0\nTerminated\n");
            return 0;
        }
        printf("Don't have any invalid layers\n");




        //find pixel per millimeter (resolution)
        printf("\n");
        int pixPerReal,maxWidth,maxHeight;
        if(modeOfRes == QString("--auto-scale"))
        {
            printf("Set auto scale based on layer's images : %s \n",raw.relate[0].toStdString().c_str());
            QImage im(raw.relate[0]);
            maxWidth = im.width();
            maxHeight = im.height();
            pixPerReal = findRatio(im,realWidth,realHeight);
        }
        else if(has_only_digits(modeOfRes.toStdString()))
        {
            printf("Set resulotion to be fixed\n");
            pixPerReal = modeOfRes.toInt();
            maxWidth = pixPerReal*realWidth;
            maxHeight = pixPerReal*realHeight;
        }else{
            printf("mode of resolution is neither integer value nor --auto-scale\nTerminated\n");
            return 0;
        }


        //show all properties
        printf("Set each layer have maximum width : %d \n",maxWidth);
        printf("Set each layer have maximum height : %d \n",maxHeight);
        printf("Set %d pixels per one millimeter.\n", pixPerReal);
        printf("\n");





        //output as visualize
        if(cmd == QString("visualize"))
        {
            w.real2Scale = pixPerReal;
            w.maxWidth = maxWidth;
            w.maxHeight = maxHeight;
            w.setLayerDiff(raw.nextReal);
            addAllImages(w,raw.relate);

            printf("visualize...\n");
            w.visualize();
            w.show();
            return a.exec();
        }


        //output as export O3DP
        else if(cmd == QString("exportO3DP"))
        {
            PixelBox pb;

            //find the maximum z input grid
            int maxZ = fromStartToDest(raw.nextReal);

            //allocate box with number of planes equal to z input grid
            pb.allocatePlane(maxWidth,maxHeight,maxZ);

            //add image to all planes
            int basedHeight = 0;
            pb.addImageToPlane(QImage(raw.relate[0]),basedHeight);
            basedHeight+=1;
            foreach(auto it,raw.nextReal){
                int count = 1;
                while(count != it.second){
                    count++;
                    basedHeight++;
                }
                pb.addImageToPlane(QImage(raw.relate[it.first+1]),basedHeight);
            }

            //write all planes to output grid
            float gridSubXY = gridXY_mm;
            int maxGridX = (int) (realWidth / gridSubXY);
            int maxGridY = (int) (realHeight / gridSubXY);
            float gridSubZ = gridZ_mm;
            int maxGridZ = (int) ((maxZ-1) / gridSubZ)+1;
            pb.writeO3DP(outfile,maxGridX,maxGridY,maxGridZ);
        }
    }
    else{
        printf("You don't write in format as description. Please --help to see all configs.\n");
    }
}
