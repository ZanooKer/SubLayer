#include "pictureedit.h"


pictureEdit::pictureEdit(QGraphicsItem *parent)
{

}

void pictureEdit::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasColor()) {
        event->setAccepted(true);
        dragOver = true;
        update();
    } else {
        event->setAccepted(false);
    }
}

void pictureEdit::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    dragOver = false;
    update();
}

void pictureEdit::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    dragOver = false;
    if (event->mimeData()->hasColor())
        color = qvariant_cast<QColor>(event->mimeData()->colorData());
    update();
}
