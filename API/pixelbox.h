#ifndef PIXELBOX_H
#define PIXELBOX_H

#define MM_TO_INCH 0.0393701
#define DPI 500

#include "pixelplane.h"

class PixelBox
{
public:
    PixelBox();
    void addImageToPlane(QImage img, int realH);
    void allocatePlane(int width,int height,int top);
    void writeO3DP(QString outfile,int w,int l,int h);

private:
    std::vector<PixelPlane> planes;
    int width;
    int height;
    int length;
};

#endif // PIXELBOX_H
