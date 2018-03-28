#ifndef PIECEPICT_H
#define PIECEPICT_H

#include <QImage>

class PiecePict
{

public:
    PiecePict();
    ~PiecePict();
    void AddToLayer(QImage img,int layer);

private:
    std::list<QImage*> listImg;

};

#endif // PIECEPICT_H
