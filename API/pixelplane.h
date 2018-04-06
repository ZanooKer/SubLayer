#ifndef PIXELPLANE_H
#define PIXELPLANE_H

#include <QImage>

class PixelPlane
{
public:
    PixelPlane(int minW, int minH, int maxW, int maxH);
    bool AddImage(QImage in);

private:
    QImage image;
    int minHeightGrid, minWidthGrid, maxHeightGrid, maxWidthGrid;
};

#endif // PIXELPLANE_H
