#ifndef MESH3D_H
#define MESH3D_H

#include <fstream>
#include <sstream>
#include "mesh.h"

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

#endif // MESH3D_H
