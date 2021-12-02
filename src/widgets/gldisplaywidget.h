#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>


class Mesh;

class GLDisplayWidget : public QGLWidget {

public:
    explicit GLDisplayWidget(QWidget *parent = 0);
    ~GLDisplayWidget();

    /**
     * The scene may be initialized in this function since the mesh
     * is a data member...
     */
    void initializeGL();
    // Display the GL scene
    void paintGL();
    void resizeGL(int width, int height);
    // Display plain faces or wireframe
    void toggleDisplayType();

    /**
     * Change the type of mesh to display: current mesh is destroyed
     * and a new one is built.
    */
    void switchMesh(int index);

    /**
     * Modify mesh colors based on the type of selected curvature: mean
     * curvature, mean curvature in the X/Y/Z axis.
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
    QTimer m_timer;             // Timer to update the scene
    float m_x, m_y, m_z;        // Translation
    float m_x_angle, m_y_angle; // Rotation
    int m_curve_axis;
    int m_mesh_type;
    bool m_wireframe_display;

    QPoint m_last_mouse_pos;    // To keep the last position of the mouse

    /**
     * The object to be displayed, may be replaced by a scene if there
     * are several objects...
     */
    Mesh * m_mesh;
};

#endif  // GLDISPLAYWIDGET_H
