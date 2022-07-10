#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>

#include "rendering/mesh_rendering.h"


Scene::Scene(QWidget *parent) : QGLWidget(parent) {
    // Update the scene
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_timer.start(16);
    m_projection = glm::mat4(1.0);
    m_fov = 45.f;
}

Scene::~Scene() {}


void Scene::initializeGL() {
    // Background color
    glClearColor(0.2, 0.2, 0.2, 1);
    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}


void Scene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Color for your mesh
    glColor3f(0, 1 ,0);

    std::shared_ptr<Mesh> mesh = m_mesh.lock();
    std::shared_ptr<MeshConfig> mesh_config = m_mesh_config.lock();
    std::shared_ptr<std::unordered_map<Face_Hash, glm::vec3>> voronoi_pts
        = m_voronoi_pts.lock();
    switch (mesh_config->dimension) {
        case Dimension::D2:
            switch(mesh_config->algorithm_2d_type) {
                case Algorithm2DType::Parabola:
                    switch(mesh_config->mesh_display_type) {
                        case MeshDisplayType::PlainFaces:
                            draw_mesh_vertices_colors(&*mesh);
                            break;
                        case MeshDisplayType::Wireframe:
                            draw_mesh_wireframe_vertices_color(&*mesh);
                            break;
                    }
                case Algorithm2DType::Crust:
                    switch(mesh_config->mesh_display_type) {
                            case MeshDisplayType::PlainFaces:
                                draw_mesh_faces_colors((Mesh2D*)&*mesh);
                                break;
                            case MeshDisplayType::Wireframe:
                                draw_mesh_wireframe_faces_color(
                                    (MeshCrust*)&*mesh
                                );
                                break;
                    }
                    break;
                case Algorithm2DType::Ruppert:
                    draw_mesh_wireframe_faces_color((MeshRuppert*)&*mesh);
                    break;
                default:
                    switch(mesh_config->mesh_display_type) {
                        case MeshDisplayType::PlainFaces:
                            draw_mesh_faces_colors((Mesh2D*)&*mesh);
                            break;
                        case MeshDisplayType::Wireframe:
                            draw_mesh_wireframe_faces_color(
                                (Mesh2D*)&*mesh,
                                mesh_config->show_voronoi_display
                            );
                            break;
                    }
            }
            break;
        case Dimension::D3:
            switch(mesh_config->mesh_display_type) {
                case MeshDisplayType::PlainFaces:
                    draw_mesh_vertices_colors(&*mesh);
                    break;
                case MeshDisplayType::Wireframe:
                    draw_mesh_wireframe_vertices_color(&*mesh);
                    break;
            }
            break;
    }
    if (mesh_config->show_voronoi_display) {
        draw_voronoi_wireframe(
            (Mesh2D*)mesh.get(),
            *voronoi_pts
        );
    }
}


void Scene::resizeGL(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glm::perspective(glm::radians(m_fov), (float)width/(float)height, 0.1f, 100.f);
    glLoadMatrixf(&m_projection[0].x);
    updateGL();
}

void Scene::set_mesh(std::shared_ptr<Mesh> &mesh) {
    m_mesh = mesh;
    BoundingBox bb = mesh->get_bounding_box();

    // Place camera and ensure the mesh's bounding box is entirely visible.
    glm::vec3 pivot_point(
        -(bb.min.x + bb.max.x) / 2.f,
        -(bb.min.x + bb.max.x) / 2.f,
        -(bb.min.x + bb.max.x) / 2.f
    );

    float bb_width = bb.max.x - bb.min.x;
    float bb_height = bb.max.y - bb.min.y;
    float dist;
    if (m_height * bb_height > m_width * bb_width)
        dist = (bb_height / 2.f) / m_fov;
    else
        dist = (bb_width / 2.f) / m_fov;

    m_camera.initialize_position(pivot_point, dist);
    m_camera.update_view_matrix();
}

void Scene::set_voronoi_points(
    std::shared_ptr<std::unordered_map<Face_Hash, glm::vec3>> &voronoi_pts
) {
    m_voronoi_pts = voronoi_pts;
}

void Scene::set_mesh_config(std::shared_ptr<MeshConfig> &mesh_config) {
    m_mesh_config = mesh_config;
}


// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void Scene::mousePressEvent(QMouseEvent *event) {
    if( event != NULL )
        m_last_mouse_pos = event->pos();
}


// Mouse movement management
void Scene::mouseMoveEvent(QMouseEvent *event) {
    int dx = (event->x() - m_last_mouse_pos.x()) * MOVE_SENSITIVITY;
    int dy = (event->y() - m_last_mouse_pos.y()) * MOVE_SENSITIVITY;

    if( event != NULL ) {
        m_last_mouse_pos = event->pos();
        m_camera.rotate(dx, dy);
        m_camera.update_view_matrix();
        updateGL();
    }
}


// Mouse management for the zoom
void Scene::wheelEvent(QWheelEvent *event) {
    QPoint num_degrees = event->angleDelta();
    float zoom = ZOOM_SENSITIVITY;
    if (!num_degrees.isNull()) {
        if (num_degrees.x() < 0 && num_degrees.y() < 0) {
            zoom *= -1;
        }
        m_camera.zoom(zoom);
        m_camera.update_view_matrix();
        updateGL();
    }
}
