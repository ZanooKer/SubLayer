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
    for(int i=planes.size();i<=top;i++)
    {
        PixelPlane pp(0,0,width,length);
        planes.push_back(pp);
    }
    this->width = width;
    this->length = length;
    this->height = top;
}

void PixelBox::addImageToPlane(QImage img, int realH)
{
    planes[realH].addImage(img);
}

void PixelBox::writeO3DP(QString outfile,int w,int l,int h)
{
    printf("Write O3DP file: ...\n");
    std::ofstream os(outfile.toStdString().c_str(), std::ios::binary);
    const static std::string cookie("#OpenFab3DP V1.0 Binary");
    os.write(cookie.c_str(), cookie.size());

    printf("- Write header\n");
    //write size of outGrid
    _WriteElement(os, w);
    _WriteElement(os, l);
    _WriteElement(os, h);

    //write size of box
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);

    //write number of material
    _WriteElement(os,(int)1);

    for(int i = 0;i<h+1;i++){
        printf("Write output-grid Z: %d ",i);
        int ratioH = h/height;
        int idx = i/ratioH;
        if(i % ratioH == 0)
        {
            printf("-> correspond to input-grid Z : %d\n",idx);
            planes[idx].writeFile(os,w,l);
        }
        else
        {
            printf("-> tranparent \n");
            for(int j=0;j<l;j++)
            {
                for(int k=0;k<w;k++){
                    _WriteElement(os,(char)0);
                }
            }
        }
    }
}
