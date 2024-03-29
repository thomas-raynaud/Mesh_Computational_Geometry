#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <unordered_map>
#include <cfloat>
#include <sstream>
#include <functional>

#include "point.h"
#include "color.h"



class Vertex {

    Point _point;               // Coordonnées
    int _face;                  // Face incidente au sommet
    std::array<double, 3> _rgb; // couleur du sommet
    int _idx;                   // Indice du sommet dans le mesh
    bool _isFictive;            // true si sommet infini
    bool _display;

public:
    Vertex(Point p, int face, int idx, bool isFictive=false) : _point(p), _face(face), _idx(idx),
        _isFictive(isFictive), _display(true) {}
    Vertex() {}

    // get
    Point point() const { return _point; }
    int face() const { return _face; }
    int idx() const { return _idx; }
    bool isFictive() const { return _isFictive; }
    bool isVisible() const { return _display; }
    std::array<double, 3> color() const { return _rgb; }
    // set
    void setIdx(int idx) { _idx = idx; }
    void setFace(int face) { _face = face; }
    void setColor(std::array<double, 3> rgb) { _rgb = rgb; }
    void setPoint(Point p) {_point = p; }
    void setDisplay(bool display) {_display = display; }
};



class Face {

    std::array<int, 3> _vertices;       // Sommets de la face
    std::array<int, 3> _adjacentFaces;  // Faces adjacentes à la face.
    std::array<double, 3> _rgb;         // couleur de la face
    int _idx;                           // Indice de la face dans le mesh
    // 1e face = face opposée au 1er sommet, etc...

public:
    Face(std::array<int, 3> vertices,
         std::array<int, 3> adjacentFaces={-1,-1,-1},
         int idx = 0
         ): _vertices(vertices), _adjacentFaces(adjacentFaces), _idx(idx) {
        setRandomColor();
        this->_idx=idx;

    }
    Face() {
        setRandomColor();
    }

    //get
    const std::array<int, 3> vertices() const { return _vertices; }
    const std::array<int, 3> adjacentFaces() const { return _adjacentFaces; }
    std::array<double, 3> color() const { return _rgb; }
    int idx() const { return _idx; }

    //Set
    void setIdx(int i){_idx=i;}
    void setVertices(std::array<int,3> vertices){_vertices=vertices; }
    void setAdjacentFaces(std::array<int,3> adjacentFaces){_adjacentFaces=adjacentFaces; }
    void setRandomColor() {
        _rgb = {((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX))};
    }

    // Ajouter la face adjacente faceIndex opposée au sommet numéro pos
    void setAdjacentFace(int faceIndex, int pos) { _adjacentFaces[pos] = faceIndex; }
    void setVertice(int vIndex, int pos){_vertices[pos]=vIndex; }

};


struct Edge {
    int ve, fe, v1, v2;
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
    
    // Détecter et connecter les faces adjacentes du mesh
    void connectAdjacentFaces();
    // Calculer les Laplaciens de chaque sommet
    std::vector<Point> getLaplacians();
    // Calculer la couleur des sommets en fonction de la courbure moyenne
    void computeColors(int curveAxis);

    bool isFaceFictive(const int face) const {
        std::array<int, 3> faceVertices = faceTab[face].vertices();
        return vertexTab[faceVertices[0]].isFictive() ||
                vertexTab[faceVertices[1]].isFictive() ||
                vertexTab[faceVertices[2]].isFictive();
    }

    bool isFaceVisible(const int face) const {
        std::array<int, 3> faceVertices = faceTab[face].vertices();
        return !vertexTab[faceVertices[0]].isFictive() && vertexTab[faceVertices[0]].isVisible() &&
                !vertexTab[faceVertices[1]].isFictive() && vertexTab[faceVertices[1]].isVisible() &&
                !vertexTab[faceVertices[2]].isFictive() && vertexTab[faceVertices[2]].isVisible();
    }

    int getNbVertices() { return vertexTab.size(); }

    double getEdgeLength(Edge e);

    std::array<int,3> edgeCollapse(int, int);
    void simplify(int);
    void edgeExtend(int, int, int);
    void vertexPop(int vIdx);
    void facePop(int fIdx);

    friend class Iterator_on_faces;
    Iterator_on_faces faces_begin();
    Iterator_on_faces faces_end();

    friend class Iterator_on_vertices;
    Iterator_on_vertices vertices_begin();
    Iterator_on_vertices vertices_past_the_end();

    friend class Circulator_on_faces;
    Circulator_on_faces incident_faces(Vertex &);
    // Get circulator starting at face face_start
    Circulator_on_faces incident_faces(Vertex &, int face_start);

    friend class Circulator_on_vertices;
    Circulator_on_vertices neighbour_vertices(Vertex &);

    friend std::ostream& operator<<(std::ostream &strm, const Mesh &m);
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
    Circulator_on_faces& operator--() {
        idx--;
        if (idx < 0) {
            if (incidentFaces.size() == 0) idx = 0;
            else idx = incidentFaces.size() - 1;
        }
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
