#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <float.h>
#include <sstream>
#include <unordered_map>
#include <fstream>

#include "point.h"
#include "color.h"



class Vertex {

    Point _point;               // Coordonnées
    int _face;                  // Face incidente au sommet
    std::array<double, 3> _rgb; // couleur du sommet
    int _idx;                   // Indice du sommet dans le mesh

public:
    Vertex(Point p, int face, int idx) : _point(p), _face(face), _idx(idx) {}
    Vertex() {}

    // get
    Point point() const { return _point; }
    int face() const { return _face; }
    int idx() const { return _idx; }
    std::array<double, 3> color() const { return _rgb; }
    // set
    void setFace(int face) { _face = face; }
    void setColor(std::array<double, 3> rgb) { _rgb = rgb; }
};



class Face {

    std::array<int, 3> _vertices;       // Sommets de la face
    std::array<int, 3> _adjacentFaces;  // Faces adjacentes à la face.
    std::array<double, 3> _rgb; // couleur de la face
    // 1e face = face opposée au 1er sommet, etc...

public:
    Face(std::array<int, 3> vertices,
         std::array<int, 3> adjacentFaces={-1,-1,-1}
         ): _vertices(vertices), _adjacentFaces(adjacentFaces) {
        setRandomColor();
    }
    Face() {
        setRandomColor();
    }

    //get
    const std::array<int, 3> vertices() const { return _vertices; }
    const std::array<int, 3> adjacentFaces() const { return _adjacentFaces; }
    std::array<double, 3> color() const { return _rgb; }

    //Set
    void setVertices(std::array<int,3> vertices){_vertices=vertices; }
    void setAdjacentFaces(std::array<int,3> adjacentFaces){_adjacentFaces=adjacentFaces; }
    void setRandomColor() {
        _rgb = {((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX))};
    }

    // Ajouter la face adjacente faceIndex opposée au sommet numéro pos
    void setAdjacentFace(int faceIndex, int pos) { _adjacentFaces[pos] = faceIndex; }
    bool isFictive() {
        return _vertices[0] == -1 || _vertices[1] == -1 || _vertices[2] == -1;
    }
};



class Iterator_on_faces;      // Itérer sur les faces du mesh
class Iterator_on_vertices;   // Itérer sur les sommets du mesh
class Circulator_on_faces;    // Itérer sur les faces incidentes à un sommet
class Circulator_on_vertices; // Itérer sur les sommets adjacents à un sommet



class Mesh {

protected:
    QVector<Vertex> vertexTab;       // "Sac" de sommets
    QVector<Face> faceTab;           // "Sac" de faces
    std::vector<Point> laplacianTab; // Vecteurs laplaciens de chaque sommet

public:
    Mesh();
    virtual ~Mesh()=0;

    virtual void drawMesh();          // Afficher les faces du mesh
    virtual void drawMeshWireFrame(); // Afficher les arêtes du mesh

    void insertion(Point p); // Prend un point et l'insert à la structure
    
    // Détecter et connecter les faces adjacentes du mesh
    void connectAdjacentFaces();
    // Calculer les Laplaciens de chaque sommet
    std::vector<Point> getLaplacians();
    // Calculer la couleur des sommets en fonction de la courbure moyenne
    void computeColors(int curveAxis);


    friend class Iterator_on_faces;
    Iterator_on_faces faces_begin();
    Iterator_on_faces faces_end();

    friend class Iterator_on_vertices;
    Iterator_on_vertices vertices_begin();
    Iterator_on_vertices vertices_past_the_end();

    friend class Circulator_on_faces;
    Circulator_on_faces incident_faces(Vertex &);

    friend class Circulator_on_vertices;
    Circulator_on_vertices neighbour_vertices(Vertex &);

    friend std::ostream& operator<<(std::ostream &strm, const Mesh &m);
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


class Mesh2D : public Mesh { // Mesh sur une surface 2D (TP2)
public:
    Mesh2D();
    virtual ~Mesh2D() {}

    void flipEdge(const int &f1, const int &f2);
    void splitTriangle(int vertexIndex, int faceIndex); // Sépare une face en trois quand un nouveau sommet est dedans

    void flipRandomEdge();
    void splitRandomTriangle();

    virtual void drawMesh();
    virtual void drawMeshWireFrame();
};


class Parabola : public Mesh{ // Mesh construit à partir de (x,y) |--> (x^2+y^2)
public: Parabola();
    virtual ~Parabola() {}
};


class Iterator_on_faces {

    Face* p;

public:
    Iterator_on_faces() : p(nullptr) {}
    Iterator_on_faces(Face* x) : p(x) {}

    bool operator!=(const Iterator_on_faces& it) const { return p != it.p; }
    bool operator==(const Iterator_on_faces& it) const { return p == it.p; }

    Face& operator*()  { return *p; }
    Face* operator->() { return p;  }

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

    Vertex& operator*()  { return *p; }
    Vertex* operator->() { return p;  }

    Iterator_on_vertices& operator++() { ++p; return *this; }
    Iterator_on_vertices operator++(int) {
       Iterator_on_vertices temp = *this;
       ++(*this);
       return temp;
    }
};



class Circulator_on_faces {

    std::vector<Face*> incidentFaces; // Faces incidentes à un sommet
    int idx;                          // Indice actuel sur le tableau incidentFaces

public:
    Circulator_on_faces(std::vector<Face*> adjacent_faces): incidentFaces(adjacent_faces), idx(0) {}
    Circulator_on_faces() : idx(-1) {}

    bool operator!=(const Circulator_on_faces& it) const {
        return incidentFaces != it.incidentFaces || idx != it.idx;
    }

    Face& operator*() { return *(incidentFaces[idx]); }
    Face* operator->() { return incidentFaces[idx]; }


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

    std::vector<Vertex*> neighborVertices; // Sommets voisins
    int idx;                               // Indice actuel sur le tableau adjacentVertices

public:
    Circulator_on_vertices(std::vector<Vertex*> neighbor_vertices): neighborVertices(neighbor_vertices), idx(0) {}
    Circulator_on_vertices() : idx(-1) {}

    bool operator!=(const Circulator_on_vertices& it) const {
        return neighborVertices != it.neighborVertices || idx != it.idx;
    }

    Vertex& operator*() { return *(neighborVertices[idx]); }
    Vertex* operator->() { return neighborVertices[idx]; }


    Circulator_on_vertices& operator++() {
        idx = (idx + 1) % neighborVertices.size();
        return *this;
    }
    Circulator_on_vertices& operator--() {
        idx--;
        if (idx < 0) {
            if (neighborVertices.size() == 0) idx = 0;
            else idx = neighborVertices.size() - 1;
        }
        return *this;
    }
    Circulator_on_vertices operator++(int) {
       Circulator_on_vertices temp = *this;
       ++(*this);
       return temp;
    }
};

std::ostream& operator<<(std::ostream &strm, const Mesh &m);

#endif // MESH_H
