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
    for(int i=planes.size();i<top;i++)
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

void PixelBox::writeO3DP(QString outfile ,int maxGridX,int maxGridY,int maxGridZ)
{
    printf("Write O3DP file: ...\n");
    std::ofstream os(outfile.toStdString().c_str(), std::ios::binary);
    const static std::string cookie("#OpenFab3DP V1.0 Binary");
    os.write(cookie.c_str(), cookie.size());

    printf("Grid X : %d -> %d\n",width,maxGridX);
    printf("Grid Y : %d -> %d\n",length,maxGridY);
    printf("Grid Z : %d -> %d\n",height,maxGridZ);
    printf("- Write header -\n");
    //write size of outGrid
    printf("Write output grid : %d %d %d \n",maxGridX,maxGridY,maxGridZ);
    _WriteElement(os, maxGridX);
    _WriteElement(os, maxGridY);
    _WriteElement(os, maxGridZ);

    //write size of box
    printf("Write box (deprecated)\n");
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);
    _WriteElement(os, (double)0);

    //write number of material
    printf("Write number of material: \n");
    _WriteElement(os,(uint32_t)7);

    printf("- Write material in each grid -\n");
    if(maxGridZ == 1){
        planes[0].writeFile(os,maxGridX,maxGridY);
    }
    else{
        for(int i = 0;i<maxGridZ;++i){
            int temp = i*(height-1);
            if(temp % (maxGridZ-1) == 0){
                printf("input grid Z %d --- output grid Z %d\n",(temp / (maxGridZ-1)),i);
                planes[temp / (maxGridZ-1)].writeFile(os,maxGridX,maxGridY);

            }else{
                printf("transparent layer --- output grid Z %d\n",i);
                for(int j = 0;j<maxGridY;++j){
                    for(int k = 0;k<maxGridX;++k){
                        _WriteElement(os,(char)0);
                    }
                }
            }
        }

    }
}
