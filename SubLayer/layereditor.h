#ifndef LAYEREDITOR_H
#define LAYEREDITOR_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include "picturepiece.h"


class LayerEditor : public QGraphicsScene
{
public:
    LayerEditor();
    void addItemFromLayer(std::list<PicturePiece> pps, int num);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
};

#endif // LAYEREDITOR_H
