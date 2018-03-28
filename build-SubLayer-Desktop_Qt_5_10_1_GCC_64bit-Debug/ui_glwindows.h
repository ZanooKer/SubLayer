/********************************************************************************
** Form generated from reading UI file 'glwindows.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLWINDOWS_H
#define UI_GLWINDOWS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GLWindows
{
public:

    void setupUi(QWidget *GLWindows)
    {
        if (GLWindows->objectName().isEmpty())
            GLWindows->setObjectName(QStringLiteral("GLWindows"));
        GLWindows->resize(400, 300);

        retranslateUi(GLWindows);

        QMetaObject::connectSlotsByName(GLWindows);
    } // setupUi

    void retranslateUi(QWidget *GLWindows)
    {
        GLWindows->setWindowTitle(QApplication::translate("GLWindows", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GLWindows: public Ui_GLWindows {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLWINDOWS_H
