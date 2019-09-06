#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <math.h>
#include <QGLWidget>

class Point {
protected:
    double _x;
    double _y;
    double _z;


public:
    Point():_x(0),_y(0),_z(0) {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
    // get
    virtual double x() const { return _x; }
    virtual double y() const { return _y; }
    virtual double z() const { return _z; }
    Point difference(Point p) {
        Point diffP(_x - p.x(), _y - p.y(), _z - p.z());
        return diffP;
    }
    double norm() {
        return sqrt(_x * _x + _y * _y + _z * _z);
    }
};

class Vertex {
    Point _point;
    int _face;
public:
    Vertex(Point p, int face): _point(p), _face(face) {}
    Vertex() {}
    // get
    Point point() const { return _point; }
    int face() const { return _face; }
    // set
    void setFace(int face) { _face = face; }
};

class Face {
    std::array<int, 3> _vertices;
    std::array<int, 3> _adjacentFaces;
public:
    Face(std::array<int, 3> vertices, std::array<int, 3> adjacentFaces={-1,-1,-1}) : _vertices(vertices), _adjacentFaces(adjacentFaces) {}
    Face() {}
    void addAdjacentFace(int faceIndex, int pos) {
        _adjacentFaces[pos] = faceIndex;
    }
    //get
    const std::array<int, 3> vertices() const { return _vertices; }
    const std::array<int, 3> adjacentFaces() const { return _adjacentFaces; }
};

Point produitVectoriel(Point u, Point v);

#endif // MATH_UTIL_H
