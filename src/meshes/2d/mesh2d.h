#ifndef MESH2D_H
#define MESH2D_H

#include "meshes/mesh.h"

#include <set>

#include "techniques/predicate.h"


class Mesh2D : public Mesh { // Mesh sur une surface 2D

protected:
    int _inf_v; // Index of infinite vertex
    QVector<Vertex> _vVertices; // Vertex de Voronoi, ce vector est indexé de la même manière que faces
    std::set<int> m_hidden_vertices;

public:
    Mesh2D();
    virtual ~Mesh2D() {}

    //Tout ce qui concerne Voronoi
    QVector<Vertex> vVertices(){return _vVertices;}
    void buildVoronoi();


    void flipEdge(const int &f1, const int &f2);
    void splitTriangle(int vertexIndex, int faceIndex); // Sépare une face en trois quand un nouveau sommet est dedans

    int takeStepVisibilityMarch(const int& faceIdx, const int& vertexIdx);
    void insertion(Point p); // Prend un point et l'insert à la structure
    // Réarranger les triangles du maillage après insertion du sommet vertexId
    // pour que la triangulation soit globalement de Delaunay
    void rearrangeDelaunay(int vertexIdx);

    Point computeCenter(Point, Point, Point);

    const bool is_vertex_fictive(const Vertex &v) const {
        return v.idx() == _inf_v;
    }

    const bool is_vertex_visible(const Vertex &v) const {
        return m_hidden_vertices.find(v.idx()) == m_hidden_vertices.end();
    }

    const bool isFaceFictive(const int face) const {
        std::array<int, 3> face_vertices = faceTab[face].vertices();
        return  is_vertex_fictive(m_vertices[face_vertices[0]]) ||
                is_vertex_fictive(m_vertices[face_vertices[1]]) ||
                is_vertex_fictive(m_vertices[face_vertices[2]]);
    }

    const bool isFaceVisible(const int face) const {
        std::array<int, 3> face_vertices = faceTab[face].vertices();
        return  !is_vertex_visible(m_vertices[face_vertices[0]]) ||
                !is_vertex_visible(m_vertices[face_vertices[1]]) ||
                !is_vertex_visible(m_vertices[face_vertices[2]]);
    }

    virtual void drawMesh() override;
    virtual void drawMeshWireFrame() override;
    virtual void drawVoronoiWireFrame();

    friend std::ostream& operator<<(std::ostream &strm, const Mesh2D &m);
};

std::ostream& operator<<(std::ostream &strm, const Mesh2D &m);

#endif  // MESH2D_H