#include "gldisplaywidget.h"
#ifdef __APPLE__
#include <glu.h>
#else
    #include <GL/glu.h>
#endif

#include "QDebug"

GLDisplayWidget::~GLDisplayWidget() {
    delete _mesh;
}

GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent) {
    // Update the scene
    connect( &_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(16);
    _wireframe_display = false;
    _curveAxis = 0;
    _meshType = 0;
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Construction of the mesh before it is displayed
    //_mesh = new Tetrahedron();
    _mesh = new Tetrahedron();
    _mesh->computeColors(_curveAxis);
}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,  0,0,0,   0,1,0);

    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angleY, 1.0, 0.0, 0.0f);
    glRotatef(_angleX, 0.0, 1.0, 0.0f);

    // Color for your mesh
    glColor3f(0, 1 ,0);

    // example with a tetraedre
    if (_wireframe_display) _mesh->drawMeshWireFrame();
    else _mesh->drawMesh();
}

void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

void GLDisplayWidget::toggleDisplayType() {
    _wireframe_display = !_wireframe_display;
}

void GLDisplayWidget::switchMesh(int index) {
    delete _mesh;
    _meshType = index;
    switch(_meshType) {
        case 0:     _mesh = new Tetrahedron(); break;
        case 1:     _mesh = new Pyramid(); break;
        case 2:     _mesh = new BoundingBox2D(); break;
        case 3:     _mesh = new QueenMesh(); break;
        case 4:		_mesh = new Parabola(); break;
        default:    _mesh = new Tetrahedron(); break;
    }
    _mesh->computeColors(_curveAxis);
}

void GLDisplayWidget::switchCurveAxis(int index) {
    _curveAxis = index;
    _mesh->computeColors(_curveAxis);
}

void GLDisplayWidget::switchTP(int index) {
    if (index == 0) {
        switchMesh(_meshType);
        switchCurveAxis(_curveAxis);
    } else {
        delete _mesh;
        _mesh = new Parabola();
        //_mesh = new Mesh2D();
    }
}

void GLDisplayWidget::flipRandomEdge() {
    ((Mesh2D*)_mesh)->flipRandomEdge();
}

void GLDisplayWidget::insertPoint() {
    double rand_x = -2.0 + (((float) rand()) / (float) RAND_MAX) * 4.0;
    double rand_y = -2.0 + (((float) rand()) / (float) RAND_MAX) * 4.0;
    ((Mesh2D*)_mesh)->insertion(Point(rand_x, rand_y, 0));
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _lastPosMouse = event->pos();
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    int dy = event->y() - _lastPosMouse.y();

    if( event != NULL )
    {
        _angleX += dx;
        _angleY += dy;
        _lastPosMouse = event->pos();

        updateGL();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.25;
    if (!numDegrees.isNull())
    {
      _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}
