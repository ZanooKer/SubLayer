#include "pixelbox.h"

PixelBox::PixelBox()
{

}

void PixelBox::addPlane(PixelPlane plane, int layer)
{
    while(planes.size() <= layer)
    {
        PixelPlane pp(0,0,0,0);
        planes.push_back(pp);
    }
    planes[layer] = plane;
}
