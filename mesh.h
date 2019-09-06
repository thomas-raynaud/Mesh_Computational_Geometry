#ifndef MESH_H
#define MESH_H

#include <QGLWidget>

#include "math_util.h"

class Iterator_on_faces;
class Iterator_on_vertices;
class Circulator_on_faces;
class Circulator_on_vertices;

class Mesh
{
protected:
    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;


public:
    Mesh();
    virtual ~Mesh()=0;
    
    void drawMesh();
    void drawMeshWireFrame();
    QVector<QVector<double>> getLap();

    friend class Iterator_on_faces;
    Iterator_on_faces faces_begin();
    Iterator_on_faces faces_end();
    friend class Iterator_on_vertices;
    Iterator_on_vertices vertices_begin();
    Iterator_on_vertices vertices_past_the_end();
    friend class Circulator_on_faces;
    Circulator_on_faces incident_faces(Vertex &v);
    friend class Circulator_on_vertices;
    Circulator_on_vertices incident_vertices(Vertex &v);
};

class Tetrahedron : public Mesh {
public:
    Tetrahedron();
    virtual ~Tetrahedron() {}
};

class Pyramid : public Mesh {
public:
    Pyramid();
    virtual ~Pyramid() {}
};

class BoundingBox2D : public Mesh {
public:
    BoundingBox2D();
    virtual ~BoundingBox2D() {}
};

class QueenMesh : public Mesh {
public:
    QueenMesh();
    virtual ~QueenMesh() {}
};

class Iterator_on_faces {
    Face* p;
public:
    Iterator_on_faces() : p(nullptr) {}
    Iterator_on_faces(Face* x) : p(x) {}
    bool operator!=(const Iterator_on_faces& it) const;
    Face& operator*() {
        return *p;
    }
    Face* operator->() {
        return p;
    }
    Iterator_on_faces& operator++() {
        ++p; return *this;
    }
};

class Iterator_on_vertices {
    Vertex* p;
public:
    Iterator_on_vertices() : p(nullptr) {}
    Iterator_on_vertices(Vertex* x) : p(x) {}
    bool operator!=(const Iterator_on_vertices& it) const;
    Vertex& operator*() {
        return *p;
    }
    Vertex* operator->() {
        return p;
    }
    Iterator_on_vertices& operator++() {
        ++p; return *this;
    }
};

class Circulator_on_faces {
    std::vector<Face*> faces_adjacentes;
    int ind;
public:
    Circulator_on_faces(std::vector<Face*> adjacent_faces) : faces_adjacentes(adjacent_faces), ind(0) {}
    Circulator_on_faces() : ind(-1) {}

    bool operator!=(const Circulator_on_faces& it) const;
    Face& operator*() {
        return *(faces_adjacentes[ind]);
    }
    Face* operator->() {
        return faces_adjacentes[ind];
    }
    Circulator_on_faces& operator++() {
        ind = (ind + 1) % faces_adjacentes.size();
        return *this;
    }
    Circulator_on_faces operator++(int) {
       Circulator_on_faces temp = *this;
       ++*this;
       return temp;
    }
};

class Circulator_on_vertices {
    std::vector<Vertex*> sommets_adjacents;
    int ind;
public:
    Circulator_on_vertices(std::vector<Vertex*> adjacent_vertices) : sommets_adjacents(adjacent_vertices), ind(0) {}
    Circulator_on_vertices() : ind(-1) {}

    bool operator!=(const Circulator_on_vertices& it) const;
    Vertex& operator*() {
        return *(sommets_adjacents[ind]);
    }
    Vertex* operator->() {
        return sommets_adjacents[ind];
    }
    Circulator_on_vertices& operator++() {
        ind = (ind + 1) % sommets_adjacents.size();
        return *this;
    }
    Circulator_on_vertices operator++(int) {
       Circulator_on_vertices temp = *this;
       ++*this;
       return temp;
    }
};

#endif // MESH_H
