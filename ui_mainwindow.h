/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gldisplaywidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    GLDisplayWidget *widget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonDisplayType;
    QComboBox *comboBoxTPswitch;
    QComboBox *comboBoxMeshswitch;
    QComboBox *comboBoxColorswitch;
    QPushButton *pushButtonInsertPoint;
    QPushButton *pushButtonFlipEdge;
    QPushButton *pushButtonExit;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(747, 528);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new GLDisplayWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayoutWidget = new QWidget(widget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 171, 351));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButtonDisplayType = new QPushButton(verticalLayoutWidget);
        pushButtonDisplayType->setObjectName(QString::fromUtf8("pushButtonDisplayType"));

        verticalLayout->addWidget(pushButtonDisplayType);

        comboBoxTPswitch = new QComboBox(verticalLayoutWidget);
        comboBoxTPswitch->addItem(QString());
        comboBoxTPswitch->addItem(QString());
        comboBoxTPswitch->setObjectName(QString::fromUtf8("comboBoxTPswitch"));

        verticalLayout->addWidget(comboBoxTPswitch);

        comboBoxMeshswitch = new QComboBox(verticalLayoutWidget);
        comboBoxMeshswitch->addItem(QString());
        comboBoxMeshswitch->addItem(QString());
        comboBoxMeshswitch->addItem(QString());
        comboBoxMeshswitch->addItem(QString());
        comboBoxMeshswitch->addItem(QString());
        comboBoxMeshswitch->setObjectName(QString::fromUtf8("comboBoxMeshswitch"));
        comboBoxMeshswitch->setEnabled(true);

        verticalLayout->addWidget(comboBoxMeshswitch);

        comboBoxColorswitch = new QComboBox(verticalLayoutWidget);
        comboBoxColorswitch->addItem(QString());
        comboBoxColorswitch->addItem(QString());
        comboBoxColorswitch->addItem(QString());
        comboBoxColorswitch->addItem(QString());
        comboBoxColorswitch->setObjectName(QString::fromUtf8("comboBoxColorswitch"));

        verticalLayout->addWidget(comboBoxColorswitch);

        pushButtonInsertPoint = new QPushButton(verticalLayoutWidget);
        pushButtonInsertPoint->setObjectName(QString::fromUtf8("pushButtonInsertPoint"));

        verticalLayout->addWidget(pushButtonInsertPoint);

        pushButtonFlipEdge = new QPushButton(verticalLayoutWidget);
        pushButtonFlipEdge->setObjectName(QString::fromUtf8("pushButtonFlipEdge"));

        verticalLayout->addWidget(pushButtonFlipEdge);

        pushButtonExit = new QPushButton(verticalLayoutWidget);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));

        verticalLayout->addWidget(pushButtonExit);


        horizontalLayout->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);
        QObject::connect(pushButtonExit, SIGNAL(released()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Mesh_Computational_Geometry", nullptr));
        pushButtonDisplayType->setText(QCoreApplication::translate("MainWindow", "Show wireframe", nullptr));
        comboBoxTPswitch->setItemText(0, QCoreApplication::translate("MainWindow", "TP1 - mesh curves", nullptr));
        comboBoxTPswitch->setItemText(1, QCoreApplication::translate("MainWindow", "TP2 - Delaunay", nullptr));

        comboBoxMeshswitch->setItemText(0, QCoreApplication::translate("MainWindow", "Tetrahedron", nullptr));
        comboBoxMeshswitch->setItemText(1, QCoreApplication::translate("MainWindow", "Pyramid", nullptr));
        comboBoxMeshswitch->setItemText(2, QCoreApplication::translate("MainWindow", "2D bounding box", nullptr));
        comboBoxMeshswitch->setItemText(3, QCoreApplication::translate("MainWindow", "Statue (OFF file)", nullptr));
        comboBoxMeshswitch->setItemText(4, QCoreApplication::translate("MainWindow", "Parabola", nullptr));

        comboBoxColorswitch->setItemText(0, QCoreApplication::translate("MainWindow", "Mean curvature", nullptr));
        comboBoxColorswitch->setItemText(1, QCoreApplication::translate("MainWindow", "X mean curvature", nullptr));
        comboBoxColorswitch->setItemText(2, QCoreApplication::translate("MainWindow", "Y mean curvature", nullptr));
        comboBoxColorswitch->setItemText(3, QCoreApplication::translate("MainWindow", "Z mean curvature", nullptr));

        pushButtonInsertPoint->setText(QCoreApplication::translate("MainWindow", "Insert a point", nullptr));
        pushButtonFlipEdge->setText(QCoreApplication::translate("MainWindow", "Flip a random edge", nullptr));
        pushButtonExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
