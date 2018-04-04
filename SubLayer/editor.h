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

#include <QObject>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMenu>

#define         DEFAULT_ZOOM_FACTOR             1.15
#define         DEFAULT_ZOOM_CTRL_FACTOR        1.01

class Editor : public QGraphicsView
{
public:
    Editor(QWidget *parent);
    ~Editor();
    void setImage(const QImage & image);
    void setZoomFactor(const double factor) { zoomFactor=factor; }
    void setZoomCtrlFactor(const double factor) {zoomCtrlFactor=factor; }
public slots:
    void fitImage();

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void drawOnImage(QPainter* painter, QSize imageSize);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void drawInViewPort(QPainter* painter, QSize portSize);
    virtual void resizeEvent(QResizeEvent *event);
    //void mousePressEvent(QGraphicsSceneDragDropEvent *event);
    //void mouseReleaseEvent(QGraphicsSceneDragDropEvent *event);
    //void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    //void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* pixmapItem;
    QSize imageSize;
    double zoomFactor;
    double zoomCtrlFactor;
    QPixmap pixmap;
};

#endif // EDITOR_H
