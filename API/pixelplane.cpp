#include "pixelplane.h"

PixelPlane::PixelPlane(int minW,int minH,int maxW,int maxH):
    minHeightGrid(minH),minWidthGrid(minW),maxHeightGrid(maxH),maxWidthGrid(maxW)
{

}

bool PixelPlane::AddImage(QImage in)
{
    if(!(in.width() < maxWidthGrid && in.height() < maxHeightGrid))
    {
        image = in.scaled(QSize(maxWidthGrid,maxHeightGrid),Qt::KeepAspectRatio);
    }
}
