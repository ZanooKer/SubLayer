#include "pixelplane.h"
#include "QImageWriter"

template <typename T>
static void
_WriteElement(std::ostream &os, T element)
{
    os.write(reinterpret_cast<const char*>(&element), sizeof(T));
}
int toRange(int value,int min,int max){
    if(value < min)return min;
    else if(value > max)return max;
    else return value;
}

PixelPlane::PixelPlane(int minW,int minH,int maxW,int maxH):
    minHeightGrid(minH),minWidthGrid(minW),maxHeightGrid(maxH),maxWidthGrid(maxW)
{
    center = std::make_pair((minW+maxW)/2 , (minH+maxH)/2);
    QColor tran(0,0,0,0);
    QImage q(maxW,maxH,QImage::Format_ARGB32);
    image = q;
    for(int i=0;i<maxH;i++)
    {
        for(int j=0;j<maxW;j++)
        {
            image.setPixelColor(QPoint(j,i),tran);
        }
    }
}

void PixelPlane::addImage(QImage in)
{
    //Not zoom in
    QImage temp = in;
    if(!(in.width() < maxWidthGrid && in.height() < maxHeightGrid))
    {
        temp = in.scaled(QSize(maxWidthGrid,maxHeightGrid),Qt::KeepAspectRatio);
    }

    for(int i = -1*temp.height()/2;i<temp.height()/2;i++)
    {
        for(int j = -1*temp.width()/2;j<temp.width()/2;j++)
        {
            QColor c(temp.pixelColor(j+temp.width()/2,i+temp.height()/2));
            image.setPixelColor(j+center.first,i+center.second,c);
        }
    }
}

void PixelPlane::checkImage()
{
    for(int i=0;i<this->maxHeightGrid;i++)
    {
        for(int j=0;j<maxWidthGrid;j++)
        {
            if(image.pixelColor(QPoint(j,i)).alpha() == 0)
                printf("0 ");
            else printf("1 ");
        }
        printf("\n");
    }
}

void PixelPlane::writeFile(std::ostream& os,int maxX,int maxY)
{
    image = dithering(image);
    maxWidthGrid = image.width();
    maxHeightGrid = image.height();

    QImageWriter iw("dithering.png");
    iw.setOptimizedWrite( true );
    iw.setProgressiveScanWrite( true );
    iw.write(image);

    float ratY = (maxHeightGrid-1)*1.00f/(maxY-1);
    float ratX = (maxWidthGrid-1)*1.00f/(maxX-1);
    for(int i = 0;i<maxY;++i){
        for(int j=0;j<maxX;++j){
            if(image.pixelColor(QPoint((int)(j*ratX),(int)(i*ratY))).alpha() == 0){
                _WriteElement(os,(char)0);
            }else{
                int mate = 2;
                QColor c = image.pixelColor(QPoint((int)(j*ratX),(int)(i*ratY)));
                //check red -> C
                if(c.red() == 0)mate+=1;
                //check green -> m
                if(c.green() == 0)mate+=4;
                //check blue -> y
                if(c.blue() == 0)mate+=2;
                _WriteElement(os,(char)mate);
            }
        }
    }
}

QImage PixelPlane::dithering(QImage img){
    //dithering
    //increase scale
    QImage img_dit(img.width()*8,img.height()*8,QImage::Format_RGBA8888);
    for(int i = 0;i<img_dit.height();++i){
        for(int j=0;j<img_dit.width();++j){
            img_dit.setPixelColor(j,i,image.pixelColor(j/8,i/8));
        }
    }
    //use Floyd–Steinberg_dithering on each channel
    for(int ch=0;ch<3;ch++){
        for(int i = 0;i<img_dit.height();++i){
            for(int j=0;j<img_dit.width();++j){
                if(!img_dit.pixelColor(j,i).alpha() == 0){
                    if(ch == 0){
                        QColor temp = img_dit.pixelColor(j,i);
                        int oldRed = temp.red();
                        temp.setRed((temp.red() > 125)?255:0);
                        img_dit.setPixelColor(j,i,temp);

                        int quant_error = oldRed - temp.red();
                        QColor tempOld;
                        if(j+1 < img_dit.width()){
                            tempOld = img_dit.pixelColor(j+1,i);
                            tempOld.setRed(toRange(tempOld.red() + quant_error * 7 / 16,0,255));
                            if(tempOld.alpha() != 0)
                                img_dit.setPixelColor(j+1,i,tempOld);
                            if(i+1 < img_dit.height()){
                                tempOld = img_dit.pixelColor(j+1,i+1);
                                tempOld.setRed(toRange(tempOld.red() + quant_error * 1 / 16,0,255));
                                if(tempOld.alpha() != 0)
                                    img_dit.setPixelColor(j+1,i+1,tempOld);
                            }
                        }
                        if(i+1 < img_dit.height()){
                            tempOld = img_dit.pixelColor(j,i+1);
                            tempOld.setRed(toRange(tempOld.red() + quant_error * 5 / 16,0,255));
                            if(tempOld.alpha() != 0)
                                img_dit.setPixelColor(j,i+1,tempOld);
                            if(j-1 >= 0){
                                tempOld = img_dit.pixelColor(j-1,i+1);
                                tempOld.setRed(toRange(tempOld.red() + quant_error * 3 / 16,0,255));
                                if(tempOld.alpha() != 0)
                                    img_dit.setPixelColor(j-1,i+1,tempOld);
                            }
                        }
                    }
                    else if(ch == 1){
                        QColor temp = img_dit.pixelColor(j,i);
                        int oldRed = temp.green();
                        temp.setGreen((temp.green() > 125)?255:0);
                        img_dit.setPixelColor(j,i,temp);

                        int quant_error = oldRed - temp.green();
                        QColor tempOld;
                        if(j+1 < img_dit.width()){
                            tempOld = img_dit.pixelColor(j+1,i);
                            tempOld.setGreen(toRange(tempOld.green() + quant_error * 7 / 16,0,255));
                            if(tempOld.alpha() != 0)
                                img_dit.setPixelColor(j+1,i,tempOld);
                            if(i+1 < img_dit.height()){
                                tempOld = img_dit.pixelColor(j+1,i+1);
                                tempOld.setGreen(toRange(tempOld.green() + quant_error * 1 / 16,0,255));
                                if(tempOld.alpha() != 0)
                                    img_dit.setPixelColor(j+1,i+1,tempOld);
                            }
                        }
                        if(i+1 < img_dit.height()){
                            tempOld = img_dit.pixelColor(j,i+1);
                            tempOld.setGreen(toRange(tempOld.green() + quant_error * 5 / 16,0,255));
                            if(tempOld.alpha() != 0)
                                img_dit.setPixelColor(j,i+1,tempOld);
                            if(j-1 >= 0){
                                tempOld = img_dit.pixelColor(j-1,i+1);
                                tempOld.setGreen(toRange(tempOld.green() + quant_error * 3 / 16,0,255));
                                if(tempOld.alpha() != 0)
                                    img_dit.setPixelColor(j-1,i+1,tempOld);
                            }
                        }
                    }else{
                        QColor temp = img_dit.pixelColor(j,i);
                        int oldRed = temp.blue();
                        temp.setBlue((temp.blue() > 125)?255:0);
                        img_dit.setPixelColor(j,i,temp);

                        int quant_error = oldRed - temp.blue();
                        QColor tempOld;
                        if(j+1 < img_dit.width()){
                            tempOld = img_dit.pixelColor(j+1,i);
                            tempOld.setBlue(toRange(tempOld.blue() + quant_error * 7 / 16,0,255));
                            if(tempOld.alpha() != 0)
                                img_dit.setPixelColor(j+1,i,tempOld);
                            if(i+1 < img_dit.height()){
                                tempOld = img_dit.pixelColor(j+1,i+1);
                                tempOld.setBlue(toRange(tempOld.blue() + quant_error * 1 / 16,0,255));
                                if(tempOld.alpha() != 0)
                                    img_dit.setPixelColor(j+1,i+1,tempOld);
                            }
                        }
                        if(i+1 < img_dit.height()){
                            tempOld = img_dit.pixelColor(j,i+1);
                            tempOld.setBlue(toRange(tempOld.blue() + quant_error * 5 / 16,0,255));
                            if(tempOld.alpha() != 0)
                                img_dit.setPixelColor(j,i+1,tempOld);
                            if(j-1 >= 0){
                                tempOld = img_dit.pixelColor(j-1,i+1);
                                tempOld.setBlue(toRange(tempOld.blue() + quant_error * 3 / 16,0,255));
                                if(tempOld.alpha() != 0)
                                    img_dit.setPixelColor(j-1,i+1,tempOld);
                            }
                        }
                    }
                }
            }
        }
    }
    return img_dit;
}
