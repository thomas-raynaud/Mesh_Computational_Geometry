#include "gldisplaywidget.h"

#include <glm/gtc/matrix_transform.hpp>

#include "meshes/mesh.h"
#include "meshes/3d/mesh3d.h"
#include "meshes/2d/meshdelaunay.h"
#include "meshes/2d/meshparabola.h"
#include "meshes/2d/meshcrust.h"
#include "meshes/2d/meshruppert.h"


GLDisplayWidget::~GLDisplayWidget() {
    delete m_mesh;
}


GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent) {
    // Update the scene
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_timer.start(16);
    m_wireframe_display = false;
    m_curve_axis = 0;
    m_mesh_type = 0;

    m_x = 0.f;
    m_y = 0.f;
    m_z = 0.f;
    m_x_angle = 0.f;
    m_y_angle = 0.f;
}


void GLDisplayWidget::initializeGL() {
    // Background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Construction of the mesh before it is displayed
    m_mesh = new Tetrahedron();
    m_mesh->computeColors(m_curve_axis);
}


void GLDisplayWidget::paintGL() {

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

    // example with a tetraedre
    if (m_wireframe_display) m_mesh->drawMeshWireFrame();
    else m_mesh->drawMesh();
}


void GLDisplayWidget::resizeGL(int width, int height) {
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


void GLDisplayWidget::toggleDisplayType() {
    m_wireframe_display = !m_wireframe_display;
}


void GLDisplayWidget::switchMesh(int index) {
    delete m_mesh;
    m_mesh_type = index;
    switch(m_mesh_type) {
        case 0:     m_mesh = new Tetrahedron(); break;
        case 1:     m_mesh = new Pyramid(); break;
        case 2:     m_mesh = new BoundingBox2D(); break;
        case 3:     m_mesh = new QueenMesh(); break;
        default:    m_mesh = new Tetrahedron(); break;
    }
    m_mesh->computeColors(m_curve_axis);
}


void GLDisplayWidget::switchCurveAxis(int index) {
    m_curve_axis = index;
    m_mesh->computeColors(m_curve_axis);
}


void GLDisplayWidget::switchTP(int index) {
    switch (index) {
        case 0:
            switchMesh(m_mesh_type);
            switchCurveAxis(m_curve_axis);
            break;
        case 1:
            delete m_mesh;
            m_mesh = new Mesh2D();
            break;
        case 2:
            delete m_mesh;
            m_mesh = new Parabola();
            break;
        case 3:
            m_mesh = new Crust();
            break;
        case 4:
            m_mesh = new Mesh2D();
            insertNPoints(50);
            break;
        case 5:
            m_mesh = new MeshRuppert();
            break;
    }
}


void GLDisplayWidget::switchParabolaType(int type) {
    delete m_mesh;
    m_mesh = new Parabola(type);
}


void GLDisplayWidget::insertNPoints(int n) {
    double rand_x, rand_y;
    for (int i = 0; i < n; ++i) {
        rand_x = -2.0 + (((float) rand()) / (float) RAND_MAX) * 4.0;
        rand_y = -2.0 + (((float) rand()) / (float) RAND_MAX) * 4.0;
        ((Mesh2D*)m_mesh)->insertion(Point(rand_x, rand_y, 0));
    }
    ((Mesh2D*)m_mesh)->buildVoronoi();
}


void GLDisplayWidget::simplifyMesh() {
    m_mesh->simplify(m_mesh->getNbVertices() / 2);
}


void GLDisplayWidget::raffine(double alpha) {
    ((MeshRuppert*)m_mesh)->raffinement(alpha);
}


// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event) {
    if( event != NULL )
        m_last_mouse_pos = event->pos();
}


// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event) {
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
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double step_zoom = 0.25;
    if (!numDegrees.isNull()) {
      m_z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? 
                m_z + step_zoom :
                m_z - step_zoom;
    }
}
