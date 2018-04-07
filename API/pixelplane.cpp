#include "pixelplane.h"

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
