#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QtDebug>

class Editor : public QGraphicsScene
{
public:
    Editor();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
};

#endif // EDITOR_H
