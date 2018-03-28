/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_file;
    QAction *actionClear_All;
    QAction *actionSave_project;
    QAction *actionSave_project_as;
    QAction *actionExport_Multiple_PNG;
    QAction *actionExport_STL;
    QAction *actionExport_O3DP;
    QAction *actionExit;
    QWidget *centralWidgets;
    QComboBox *comboBox;
    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *BrowseLabel;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label;
    QSpinBox *spinBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 600);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1000, 600));
        MainWindow->setMaximumSize(QSize(1000, 600));
        actionNew_file = new QAction(MainWindow);
        actionNew_file->setObjectName(QStringLiteral("actionNew_file"));
        actionClear_All = new QAction(MainWindow);
        actionClear_All->setObjectName(QStringLiteral("actionClear_All"));
        actionSave_project = new QAction(MainWindow);
        actionSave_project->setObjectName(QStringLiteral("actionSave_project"));
        actionSave_project_as = new QAction(MainWindow);
        actionSave_project_as->setObjectName(QStringLiteral("actionSave_project_as"));
        actionExport_Multiple_PNG = new QAction(MainWindow);
        actionExport_Multiple_PNG->setObjectName(QStringLiteral("actionExport_Multiple_PNG"));
        actionExport_STL = new QAction(MainWindow);
        actionExport_STL->setObjectName(QStringLiteral("actionExport_STL"));
        actionExport_O3DP = new QAction(MainWindow);
        actionExport_O3DP->setObjectName(QStringLiteral("actionExport_O3DP"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidgets = new QWidget(MainWindow);
        centralWidgets->setObjectName(QStringLiteral("centralWidgets"));
        centralWidgets->setEnabled(true);
        centralWidgets->setMinimumSize(QSize(580, 0));
        comboBox = new QComboBox(centralWidgets);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(10, 50, 86, 25));
        graphicsView = new QGraphicsView(centralWidgets);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 90, 480, 430));
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMaximumSize(QSize(480, 430));
        graphicsView_2 = new QGraphicsView(centralWidgets);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(519, 90, 471, 430));
        sizePolicy.setHeightForWidth(graphicsView_2->sizePolicy().hasHeightForWidth());
        graphicsView_2->setSizePolicy(sizePolicy);
        graphicsView_2->setMaximumSize(QSize(480, 430));
        layoutWidget = new QWidget(centralWidgets);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 471, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        BrowseLabel = new QLabel(layoutWidget);
        BrowseLabel->setObjectName(QStringLiteral("BrowseLabel"));

        horizontalLayout->addWidget(BrowseLabel);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralWidgets);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(848, 10, 141, 25));
        pushButton_3 = new QPushButton(centralWidgets);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(710, 10, 131, 25));
        label = new QLabel(centralWidgets);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(520, 60, 81, 17));
        spinBox = new QSpinBox(centralWidgets);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(590, 60, 48, 26));
        MainWindow->setCentralWidget(centralWidgets);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew_file);
        menuFile->addAction(actionSave_project);
        menuFile->addAction(actionSave_project_as);
        menuFile->addSeparator();
        menuFile->addAction(actionExport_Multiple_PNG);
        menuFile->addAction(actionExport_STL);
        menuFile->addAction(actionExport_O3DP);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew_file->setText(QApplication::translate("MainWindow", "New project", nullptr));
        actionClear_All->setText(QApplication::translate("MainWindow", "Clear All", nullptr));
        actionSave_project->setText(QApplication::translate("MainWindow", "Save project", nullptr));
        actionSave_project_as->setText(QApplication::translate("MainWindow", "Save project as", nullptr));
        actionExport_Multiple_PNG->setText(QApplication::translate("MainWindow", "Export Multiple PNG", nullptr));
        actionExport_STL->setText(QApplication::translate("MainWindow", "Export STL", nullptr));
        actionExport_O3DP->setText(QApplication::translate("MainWindow", "Export O3DP", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "Grab", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "Crop", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "Select", nullptr));
        comboBox->setItemText(3, QApplication::translate("MainWindow", "Wand", nullptr));

        BrowseLabel->setText(QApplication::translate("MainWindow", "Image path", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Browse", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "Visualize", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "Resize picture", nullptr));
        label->setText(QApplication::translate("MainWindow", "no. Layer:", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
