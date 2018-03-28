#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QDir>
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

#include "picturepiece.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);
    void setCurrentPP(std::list<PicturePiece> pps);

signals:
    void clicked();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void makeObject(std::list<PicturePiece> pps);

    std::list<PicturePiece> pps;
    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    QOpenGLTexture *textures[6];
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
};
#endif // GLWIDGET_H
