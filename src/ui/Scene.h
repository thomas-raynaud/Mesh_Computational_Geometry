#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include <unordered_map>

#include "meshes/Face.h"
#include "meshes/Vertex.h"
#include "meshes/Mesh.h"
#include "MeshConfig.h"


class Mesh;

class Scene : public QGLWidget {

public:
    explicit Scene(QWidget *parent = 0);
    ~Scene();

    void initializeGL();
    // Display the GL scene
    void paintGL();
    void resizeGL(int width, int height);

    void set_mesh(std::shared_ptr<Mesh> &mesh);
    void set_voronoi_points(
        std::shared_ptr<std::unordered_map<Face_Hash, glm::vec3>> &voronoi_pts
    );
    void set_mesh_config(std::shared_ptr<MeshConfig> &mesh_config);

    void center_camera();


    // Display plain faces or wireframe
    void switch_display_type();

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
    float m_zoom;               // Distance to the center point
    float m_x_angle, m_y_angle; // Rotation
    glm::vec3 m_center;         // Center of the orbit-camera

    QPoint m_last_mouse_pos;    // To keep the last position of the mouse

    /**
     * The object to be displayed, may be replaced by a scene if there
     * are several objects.
     */
    std::weak_ptr<Mesh> m_mesh;
    std::weak_ptr<std::unordered_map<Face_Hash, glm::vec3>> m_voronoi_pts;
    std::weak_ptr<MeshConfig> m_mesh_config;
};

#endif  // SCENE_H