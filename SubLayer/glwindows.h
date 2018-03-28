#ifndef GLWINDOWS_H
#define GLWINDOWS_H

#include <QWidget>

class GLWidget;

namespace Ui {
class GLWindows;
}

class GLWindows : public QWidget
{
    Q_OBJECT

public:
    explicit GLWindows();
    GLWidget *glWidgets;
    ~GLWindows();

private slots:
    void setCurrentGlWidget();
    void rotateOneStep();

private:
    enum { NumRows = 2, NumColumns = 3 };

    GLWidget *currentGlWidget;
};

#endif // GLWINDOWS_H
