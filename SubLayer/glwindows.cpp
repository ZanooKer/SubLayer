#include "glwindows.h"
#include "ui_glwindows.h"

#include <QtWidgets>
#include "glwidget.h"

GLWindows::GLWindows()
{
    QGridLayout *mainLayout = new QGridLayout;
    QColor clearColor;
    clearColor.setHsv(255,255, 63);

    glWidgets = new GLWidget;
    glWidgets->setClearColor(clearColor);
    glWidgets->rotateBy(+42 * 16, +42 * 16, -21 * 16);
    mainLayout->addWidget(glWidgets, 0, 0);
    //connect(glWidgets, &GLWidget::clicked,this, &GLWindows::setCurrentGlWidget);

    setLayout(mainLayout);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GLWindows::rotateOneStep);
    timer->start(20);

    setWindowTitle(tr("Textures"));
}

GLWindows::~GLWindows()
{

}

void GLWindows::setCurrentGlWidget()
{
    currentGlWidget = qobject_cast<GLWidget *>(sender());
}

void GLWindows::rotateOneStep()
{
    glWidgets->rotateBy(0, +2 * 16, 0);
}
