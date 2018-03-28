#ifndef PICTUREEDIT_H
#define PICTUREEDIT_H

#include <QGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
class QGraphicsSceneMouseEvent;
class QParallelAnimationGroup;


class pictureEdit : public QGraphicsObject
{
public:
    pictureEdit(QGraphicsItem *parent = 0);
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    QColor color;
    bool dragOver;
};

#endif // PICTUREEDIT_H
