#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"
#include "mesh3d.h"
#include "meshdelaunay.h"
#include "meshparabola.h"
#include "meshcrust.h"
#include "meshruppert.h"

#include <iostream>

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);
    ~GLDisplayWidget();

    void initializeGL(); // The scene may be initialized in this function since the mesh is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);
    void toggleDisplayType(); // Plain faces ou wireframe

    /*
     * Changer le mesh à afficher : on détruit le mesh actuel pour en construire
     * un autre.
    */
    void switchMesh(int index);

    /*
     * On modifie les couleurs du mesh en fonction du type de la courbure
     * sélectionnée : courbure moyenne, courbure moyenne dans l'axe des X, Y, Z.
    */
    void switchCurveAxis(int index);

    void switchParabolaType(int type);
    void switchTP(int index);
    void insertNPoints(int n);
    void simplifyMesh();
    void raffine(double alpha);

protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X, _Y ,_Z; // Translation
    float _angleX, _angleY; // Rotation
    int _curveAxis;
    int _meshType;
    bool _wireframe_display;

    QPoint _lastPosMouse; // To keep the last position of the mouse

    Mesh *_mesh; // The object to be displayed, may be replaced by a scene if there are several objects....
};

#endif // GLDISPLAYWIDGET_H
