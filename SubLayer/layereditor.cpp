#include "layereditor.h"

LayerEditor::LayerEditor()
{

}

void LayerEditor::addItemFromLayer(std::list<PicturePiece> pps, int num)
{
    this->clear();
    foreach (PicturePiece p , pps)
    {
        if(p.layer == num)
        {
            this->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(p.gi)));
        }
    }
}

void LayerEditor::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint mpos = event->pos();
        QPointF temp(mpos);
        QGraphicsItem *tre = itemAt(temp,QTransform());
    }
}

void LayerEditor::mouseReleaseEvent(QMouseEvent *event)
{

}

void LayerEditor::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QPointF mpos = event->pos();
}

void LayerEditor::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QPointF mpos = event->pos();
}
