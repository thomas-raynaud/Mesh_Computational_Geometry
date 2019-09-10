#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"

#include <iostream>

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);
    ~GLDisplayWidget();

    void initializeGL(); // The scene may be initialized in this function since the mesh is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);
    void toggleDisplayType();
    void switchMesh(int index);
    void switchCurveAxis(int index);

protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X, _Y ,_Z; // Translation
    float _angle; // Rotation
    bool _wireframe_display;

    QPoint _lastPosMouse; // To keep the last position of the mouse

    Mesh *_mesh; // The object to be displayed, may be replaced by a scene if there are several objects....
};

#endif // GLDISPLAYWIDGET_H
