#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include "point.h"
#include "color.h"


class Vertex {

    Point _point;
    int _face; // Face incidente au sommet
    Point _laplacian;

public:
    Vertex(Point p, int face) : _point(p), _face(face) {}
    Vertex() {}
    // get
    Point point() const { return _point; }
    int face() const { return _face; }
    Point laplacian() const { return _laplacian; }
    // set
    void setFace(int face) { _face = face; }
    void setLaplacian(Point laplacian) { _laplacian = laplacian; }
};


class Face {

    std::array<int, 3> _vertices; // Sommets de la face
    std::array<int, 3> _adjacentFaces; // Faces adjacentes à la face.
    // 1e face = face opposée au 1er sommet, etc...
    double _curvature;
    std::array<int, 3> _color;

public:
    Face(std::array<int, 3> vertices,
         std::array<int, 3> adjacentFaces={-1,-1,-1}
         ): _vertices(vertices), _adjacentFaces(adjacentFaces) {}
    Face() {}
    void addAdjacentFace(int faceIndex, int pos); // Ajouter la face adjacente
    // faceIndex opposée au sommet numéro pos

    //get
    const std::array<int, 3> vertices() const { return _vertices; }
    const std::array<int, 3> adjacentFaces() const { return _adjacentFaces; }
    double curvature() const { return _curvature; }
    std::array<int, 3> color() const { return _color; }
    //set
    void setCurvature(double curvature) { _curvature = curvature; }
    void setColor(std::array<int, 3> color) { _color = color; }
};


class Iterator_on_faces; // Itérer sur les faces du mesh
class Iterator_on_vertices; // Itérer sur les sommets du mesh
class Circulator_on_faces; // Itérer sur les faces incidentes à un sommet
class Circulator_on_vertices; // Itérer sur les sommets adjacents à un sommet

class Mesh {

protected:
    QVector<Vertex> vertexTab; // "Sac" de sommets
    QVector<Face> faceTab; // "Sac" de faces

public:
    Mesh();
    virtual ~Mesh()=0;
    
    void connectAdjacentFaces(); // Détecter et connecter les faces adjacentes
    // du mesh
    void drawMesh(); // Afficher les faces du mesh
    void drawMeshWireFrame(); // Afficher les arêtes du mesh
    void computeLaplacians(); // Calculer les Laplaciens de chaque sommet
    void computeCurvature();
    void computeColors();

    friend class Iterator_on_faces;
    Iterator_on_faces faces_begin();
    Iterator_on_faces faces_end();
    friend class Iterator_on_vertices;
    Iterator_on_vertices vertices_begin();
    Iterator_on_vertices vertices_past_the_end();
    friend class Circulator_on_faces;
    Circulator_on_faces incident_faces(Vertex &);
    friend class Circulator_on_vertices;
    Circulator_on_vertices incident_vertices(Vertex &);
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

class QueenMesh : public Mesh { // Mesh chargé à partir d'un fichier OFF
public:
    QueenMesh();
    virtual ~QueenMesh() {}
};


class Iterator_on_faces {
    Face* p;
public:
    Iterator_on_faces() : p(nullptr) {}
    Iterator_on_faces(Face* x) : p(x) {}
    bool operator!=(const Iterator_on_faces& it) const { return p != it.p; }
    bool operator==(const Iterator_on_faces& it) const { return p == it.p; }
    Face& operator*() { return *p; }
    Face* operator->() { return p; }
    Iterator_on_faces& operator++() { ++p; return *this; }
    Iterator_on_faces operator++(int) {
       Iterator_on_faces temp = *this;
       ++(*this);
       return temp;
    }
};

class Iterator_on_vertices {
    Vertex* p;
public:
    Iterator_on_vertices() : p(nullptr) {}
    Iterator_on_vertices(Vertex* x) : p(x) {}
    bool operator!=(const Iterator_on_vertices& it) const { return p != it.p; }
    bool operator==(const Iterator_on_vertices& it) const { return p == it.p; }
    Vertex& operator*() { return *p; }
    Vertex* operator->() { return p; }
    Iterator_on_vertices& operator++() { ++p; return *this; }
    Iterator_on_vertices operator++(int) {
       Iterator_on_vertices temp = *this;
       ++(*this);
       return temp;
    }
};

class Circulator_on_faces {
    std::vector<Face*> incidentFaces; // Faces adjacentes d'un sommet
    int idx; // Indice actuel sur le tableau faces_adjacentes
public:
    Circulator_on_faces(std::vector<Face*> adjacent_faces):
        incidentFaces(adjacent_faces), idx(0) {}
    Circulator_on_faces() : idx(-1) {}

    Face& operator*() { return *(incidentFaces[idx]); }
    Face* operator->() { return incidentFaces[idx]; }

    bool operator!=(const Circulator_on_faces& it) const {
        return incidentFaces != it.incidentFaces || idx != it.idx;
    }

    Circulator_on_faces& operator++() {
        idx = (idx + 1) % incidentFaces.size();
        return *this;
    }

    Circulator_on_faces operator++(int) {
       Circulator_on_faces temp = *this;
       ++(*this);
       return temp;
    }
};

class Circulator_on_vertices {
    std::vector<Vertex*> adjacentVertices;
    int idx;
public:
    Circulator_on_vertices(std::vector<Vertex*> adjacent_vertices):
        adjacentVertices(adjacent_vertices), idx(0) {}
    Circulator_on_vertices() : idx(-1) {}

    Vertex& operator*() { return *(adjacentVertices[idx]); }
    Vertex* operator->() { return adjacentVertices[idx]; }

    bool operator!=(const Circulator_on_vertices& it) const {
        return adjacentVertices != it.adjacentVertices || idx != it.idx;
    }

    Circulator_on_vertices& operator++() {
        idx = (idx + 1) % adjacentVertices.size();
        return *this;
    }

    Circulator_on_vertices operator++(int) {
       Circulator_on_vertices temp = *this;
       ++(*this);
       return temp;
    }
};

std::array<int, 3> hsv2rgb(double h, double s, double v);

#endif // MESH_H
