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
#include "camera/ArcballCamera.h"


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

    // Display plain faces or wireframe
    void switch_display_type();

    void update_view_matrix();

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
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QTimer m_timer;             // Timer to update the scene
    float m_height;
    float m_width;
    float m_fov;
    glm::mat4 m_projection;
    ArcballCamera m_camera;

    /**
     * The object to be displayed, may be replaced by a scene if there
     * are several objects.
     */
    std::weak_ptr<Mesh> m_mesh;
    std::weak_ptr<std::unordered_map<Face_Hash, glm::vec3>> m_voronoi_pts;
    std::weak_ptr<MeshConfig> m_mesh_config;
};

#endif  // SCENE_H