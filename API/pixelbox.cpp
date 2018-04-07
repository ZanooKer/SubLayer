#include "pixelbox.h"

template <typename T>
static void
_WriteElement(std::ostream &os, T element)
{
    os.write(reinterpret_cast<const char*>(&element), sizeof(T));
}

PixelBox::PixelBox()
{

}

void PixelBox::allocatePlane(int width,int length,int top)
{
    planes.clear();
    for(int i=0;i<=top;i++)
    {
        PixelPlane pp(0,0,width,length);
        planes.push_back(pp);
    }
    this->width = width;
    this->length = length;
    this->height = top;
}

void PixelBox::addImageToPlane(QImage img, int layer)
{
    planes[layer].addImage(img);
}

void PixelBox::writeO3DP(QString outfile,int w,int l,int h)
{
    std::ofstream os(outfile.toStdString().c_str(), std::ios::binary);
    const static std::string cookie("#OpenFab3DP V1.0 Binary");
    os.write(cookie.c_str(), cookie.size());

    _WriteElement(os, width);
    _WriteElement(os, length);
    _WriteElement(os, height);

    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);


}
