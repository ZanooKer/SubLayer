#include "pixelplane.h"

template <typename T>
static void
_WriteElement(std::ostream &os, T element)
{
    os.write(reinterpret_cast<const char*>(&element), sizeof(T));
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
    float ratY = (maxHeightGrid-1)*1.00f/(maxY-1);
    float ratX = (maxWidthGrid-1)*1.00f/(maxX-1);
    for(int i = 0;i<maxY;++i){
        for(int j=0;j<maxX;++j){
            if(image.pixelColor(QPoint((int)(j*ratX),(int)(i*ratY))).alpha() == 0){
                _WriteElement(os,(char)0);
            }else{
                _WriteElement(os,(char)2);
            }
        }
    }
}
