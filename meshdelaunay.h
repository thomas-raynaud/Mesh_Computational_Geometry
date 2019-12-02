#ifndef MESHDELAUNAY_H
#define MESHDELAUNAY_H

#include <queue>
#include "mesh.h"
#include "predicate.h"

class Mesh2D : public Mesh { // Mesh sur une surface 2D

protected:
    int _inf_v; // Indice du sommet infini
    QVector<Vertex> _vVertices; // Vertex de Voronoi, ce vector est indexé de la même manière que faces

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

    virtual void drawMesh();
    virtual void drawMeshWireFrame();
    virtual void drawVoronoiWireFrame();
};

#endif // MESHDELAUNAY_H
