#ifndef PICTUREPIECE_H
#define PICTUREPIECE_H

#include <QImage>

class PicturePiece
{
public:
    PicturePiece(QImage gi, int layer);
    QImage gi;
    int layer;
};

#endif // PICTUREPIECE_H
