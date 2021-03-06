#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      clearColor(Qt::black),
      xRot(0),
      yRot(0),
      zRot(0),
      program(0)
{
    memset(textures, 0, sizeof(textures));
    winscale = 0.5f;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    for (int i = 0; i < 1; ++i)
        delete textures[i];
    delete program;
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::setCurrentPP(std::vector<PicturePiece> pps)
{
    this->pps = pps;
}

void GLWidget::setMaxSizePic(int width,int  height)
{
    this->maxWidth = width;
    this->maxHeight = height;
}

void GLWidget::setDiffHeight(std::map<int,float> layerDiff)
{
    this->ld = layerDiff;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    if((int)(pps.size()) <= 0)return;

    makeObject(pps);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc =
        "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texc.st);\n"
        "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("texture", 0);
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.ortho(-1*winscale, winscale, winscale, -1*winscale, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    for (int i = 0; i < numberOfPic; ++i) {
        textures[i]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0){
        if(winscale < 0.8f)winscale += 0.05f;
        update();
    }
    if(event->angleDelta().y() < 0){
        if(winscale > 0.2f)winscale -= 0.05f;
        update();
    }

}

void GLWidget::makeObject(std::vector<PicturePiece> pps)
{
    if(sizeof pps == 0)return;

    numberOfPic = (int)pps.size();
    GLfloat coords[numberOfPic][4][3];

    for (int j = 0; j < numberOfPic; ++j)
    {
        float grid = std::max(maxHeight,maxWidth);
        float boundW = float(pps[j].gi.width()/ grid);
        float boundH = float(pps[j].gi.height()/ grid);

        for(int s = 0; s<4; s++)
        {
            coords[j][s][0] = (s==0 || s==3)?boundW:-1*boundW;
            coords[j][s][1] = -1*ld[pps[j].layer];
            coords[j][s][2] = (s<2)?boundH:-1*boundH;
            printf("%f %f %f\n",coords[j][s][0],coords[j][s][1],coords[j][s][2]);
        }
    }
    for (int j = 0; j < numberOfPic; ++j)
        textures[j] = new QOpenGLTexture(pps[j].gi);

    QVector<GLfloat> vertData;
    for (int i = 0; i < numberOfPic; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(0.2 * coords[i][j][0]);
            vertData.append(0.2 * coords[i][j][1]);
            vertData.append(0.2 * coords[i][j][2]);

            // texture coordinate
            vertData.append(j == 0 || j == 3);
            vertData.append(j == 0 || j == 1);
        }
    }


    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}
