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
    void setCurrentPP(std::vector<PicturePiece> pps);
    void setMaxSizePic(int width,int  height);
    void setDiffHeight(std::map<int,float> layerDiff);

signals:
    void clicked();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event);

private:
    void makeObject(std::vector<PicturePiece> pps);

    std::vector<PicturePiece> pps;
    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    float winscale;
    int numberOfPic;
    int maxWidth,maxHeight;
    std::map<int,float> ld;
    QOpenGLTexture *textures[6];
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
};
#endif // GLWIDGET_H
