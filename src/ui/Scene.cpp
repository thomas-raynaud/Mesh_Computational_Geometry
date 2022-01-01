#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>

#include "rendering/mesh_rendering.h"


Scene::Scene(QWidget *parent) : QGLWidget(parent) {
    // Update the scene
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_timer.start(16);
    m_x = 0.f;
    m_y = 0.f;
    m_z = 0.f;
    m_x_angle = 0.f;
    m_y_angle = 0.f;
}

Scene::~Scene() {}


void Scene::init() {
    // Background color
    glClearColor(0.2, 0.2, 0.2, 1);
    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}


void Scene::paint() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0));

    // Translation
    glTranslated(m_x, m_y, m_z);

    // Rotation
    glRotatef(m_y_angle, 1.0, 0.0, 0.0f);
    glRotatef(m_x_angle, 0.0, 1.0, 0.0f);
    glRotatef(0, 0.0, 1.0, 0.0f);

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
                            mesh->draw_mesh_vertices_colors();
                            break;
                        case MeshDisplayType::Wireframe:
                            mesh->draw_mesh_wireframe_vertices_color();
                            break;
                }
                default:
                    switch(mesh_config->mesh_display_type) {
                        case MeshDisplayType::PlainFaces:
                            mesh->draw_mesh_faces_colors();
                            break;
                        case MeshDisplayType::Wireframe:
                            mesh->draw_mesh_wireframe_faces_color();
                            break;
                    }
            }
            break;
        case Dimension::D3:
            switch(mesh_config->mesh_display_type) {
                case MeshDisplayType::PlainFaces:
                    mesh->draw_mesh_vertices_colors();
                    break;
                case MeshDisplayType::Wireframe:
                    mesh->draw_mesh_wireframe_vertices_color();
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


void Scene::resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::perspective(
        glm::radians(45.0f),
        (GLfloat)width/(GLfloat)height,
        0.1f,
        100.0f
    );

    updateGL();
}

void Scene::set_mesh(std::shared_ptr<Mesh> &mesh) {
    m_mesh = mesh;
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
    int dx = event->x() - m_last_mouse_pos.x();
    int dy = event->y() - m_last_mouse_pos.y();

    if( event != NULL )
    {
        m_x_angle += dx;
        m_y_angle += dy;
        m_last_mouse_pos = event->pos();
        updateGL();
    }
}


// Mouse Management for the zoom
void Scene::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double step_zoom = 0.25;
    if (!numDegrees.isNull()) {
      m_z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? 
                m_z + step_zoom :
                m_z - step_zoom;
    }
}
