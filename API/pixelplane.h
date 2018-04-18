#ifndef PIXELPLANE_H
#define PIXELPLANE_H

#include <QImage>
#include <QColor>
#include <cstdlib>
#include <fstream>

class PixelPlane
{
public:
    PixelPlane(int minW, int minH, int maxW, int maxH);
    void addImage(QImage in);
    void writeFile(std::ostream &os, int outw, int outh);
    void checkImage();
    QImage dithering(QImage im);

private:
    QImage image;
    int minHeightGrid, minWidthGrid, maxHeightGrid, maxWidthGrid;
    std::pair<int,int> center;
};

#endif // PIXELPLANE_H
