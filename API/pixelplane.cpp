#include "pixelplane.h"
#include "QImageWriter"

template <typename T>
static void
_WriteElement(std::ostream &os, T element)
{
    os.write(reinterpret_cast<const char*>(&element), sizeof(T));
}

float toRange(float value,float min,float max){
    if(value < min)return min;
    else if(value > max)return max;
    else return value;
}

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

void PixelPlane::checkImage()
{
    for(int i=0;i<this->maxHeightGrid;i++)
    {
        for(int j=0;j<maxWidthGrid;j++)
        {
            if(image.pixelColor(QPoint(j,i)).alpha() == 0)
                printf("0 ");
            else printf("1 ");
        }
        printf("\n");
    }
}

void PixelPlane::writeFile(std::ostream& os,int maxX,int maxY)
{
    image = dithering(image);
    maxWidthGrid = image.width();
    maxHeightGrid = image.height();

    QImageWriter iw("dithering.png");
    iw.setOptimizedWrite( true );
    iw.setProgressiveScanWrite( true );
    iw.write(image);

    float ratY = (maxHeightGrid)*1.0f/(maxY);
    float ratX = (maxWidthGrid)*1.0f/(maxX);
//    QColor ct(255,0,0,255);
//    int mf = 2;
//    ct = ct.toCmyk();
//    if(ct.cyanF() > 0.5)mf+=1;
//    //check green -> m
//    if(ct.magentaF() > 0.5)mf+=4;
//    //check blue -> y
//    if(ct.yellowF() > 0.5)mf+=2;
//    printf("%f %f %f %d AA",ct.cyanF(),ct.magentaF(),ct.yellowF(),mf);

    for(int i = 0;i<maxY;++i){
        for(int j=0;j<maxX;++j){
           if(image.pixelColor((int)(j*ratX),(int)(i*ratY)).alpha() == 0){
                _WriteElement(os,(char)0);
            }else{
                int mate = 2;
                QColor c= image.pixelColor(QPoint((int)(j*ratX),(int)(i*ratY))).toCmyk();
                //check red -> C
                if(c.cyanF() > 0.5)
                    mate+=1;
                //check green -> m
                if(c.magentaF() > 0.5)
                    mate+=4;
                //check blue -> y
                if(c.yellowF() > 0.5)
                    mate+=2;
                _WriteElement(os,(char)mate);
            }
        }
    }
}

QImage PixelPlane::dithering(QImage img){
    //dithering
    //increase scale
    QImage img_dit(img.width()*10,img.height()*10,QImage::Format_RGBA8888);
    for(int i = 0;i<img_dit.height();++i){
        for(int j=0;j<img_dit.width();++j){
            img_dit.setPixelColor(j,i,image.pixelColor(j/10,i/10));
        }
    }
    //use Floyd–Steinberg_dithering
    for(int i = 0;i<img_dit.height();++i){
        for(int j=0;j<img_dit.width();++j){
            if(img_dit.pixelColor(j,i).alpha() != 0){
                //get new color from threshold
                QColor cymk = img_dit.pixelColor(j,i).toCmyk();
                float oldCyan = cymk.cyanF();
                float oldMa = cymk.magentaF();
                float oldYell = cymk.yellowF();
                float oldBl = cymk.blackF();
                float newC = (oldCyan > 0.5f)?1.0f:0.0f;
                float newM = (oldMa > 0.5f)?1.0f:0.0f;
                float newY = (oldYell > 0.5f)?1.0f:0.0f;
                float newK = (oldBl > 0.5f)?1.0f:0.0f;
                img_dit.pixelColor(j,i).setCmykF(newC,newM,newY,newK);

                //get diff
                float diffC = newC - oldCyan;
                float diffM = newM - oldMa;
                float diffY = newY - oldYell;
                float diffK = newK - oldBl;

                //span the diff to all pixels around this pixel with the weight according to Floyd–Steinberg_dithering
                QColor tempOld;
                if(j+1 < img_dit.width()){
                    tempOld = img_dit.pixelColor(j+1,i).toCmyk();
                    if(img_dit.pixelColor(j+1,i).alpha() == 0)
                        img_dit.pixelColor(j+1,i).setCmykF(toRange(tempOld.cyanF() + diffC * 7 /16,0,1),
                                        toRange(tempOld.magentaF() + diffM * 7 /16,0,1),
                                        toRange(tempOld.yellowF() + diffY * 7 /16,0,1),
                                        toRange(tempOld.blackF() + diffK * 7 /16,0,1));
                    if(i+1 < img_dit.height()){
                        tempOld = img_dit.pixelColor(j+1,i+1).toCmyk();
                        if(img_dit.pixelColor(j+1,i+1).alpha() == 0)
                        img_dit.pixelColor(j+1,i+1).setCmykF(toRange(tempOld.cyanF() + diffC * 1 /16,0,1),
                                        toRange(tempOld.magentaF() + diffM * 1 /16,0,1),
                                        toRange(tempOld.yellowF() + diffY * 1 /16,0,1),
                                        toRange(tempOld.blackF() + diffK * 1 /16,0,1));
                    }
                }
                if(i+1 < img_dit.height()){
                    tempOld = img_dit.pixelColor(j,i+1).toCmyk();
                    if(img_dit.pixelColor(j,i+1).alpha() == 0)
                    img_dit.pixelColor(j,i+1).setCmykF(toRange(tempOld.cyanF() + diffC * 5 /16,0,1),
                                    toRange(tempOld.magentaF() + diffM * 5 /16,0,1),
                                    toRange(tempOld.yellowF() + diffY* 5 /16,0,1),
                                    toRange(tempOld.blackF() + diffK * 5 /16,0,1));
                    if(j-1 >= 0){
                        tempOld = img_dit.pixelColor(j-1,i+1).toCmyk();
                        if(img_dit.pixelColor(j-1,i+1).alpha() == 0)
                        img_dit.pixelColor(j-1,i+1).setCmykF(toRange(tempOld.cyanF() + diffC * 3 /16,0,1),
                                        toRange(tempOld.magentaF() + diffM * 3 /16,0,1),
                                        toRange(tempOld.yellowF() + diffY * 3 /16,0,1),
                                        toRange(tempOld.blackF() + diffK * 3 /16,0,1));
                    }
                }

            }
        }
    }
    return img_dit;
}
