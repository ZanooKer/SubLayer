#include "editor.h"

Editor::Editor()
{

}

void Editor::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint mpos = event->pos();
        QPointF temp(mpos);
        QGraphicsItem *tre = itemAt(temp,QTransform());
        qDebug() << tre->pos().rx();
    }
}

void Editor::mouseReleaseEvent(QMouseEvent *event)
{

}

void Editor::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QPointF mpos = event->pos();
}

void Editor::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QPointF mpos = event->pos();
}
