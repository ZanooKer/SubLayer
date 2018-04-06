#ifndef PIXELBOX_H
#define PIXELBOX_H

#include "pixelplane.h"

class PixelBox
{
public:
    PixelBox();
    void addPlane(PixelPlane plane,int layer);

private:
    std::vector<PixelPlane> planes;
};

#endif // PIXELBOX_H
