#ifndef MESH3D_H
#define MESH3D_H

#include "meshes/Mesh.h"


class Mesh3D : public Mesh {
protected:
    // Stores a Laplacian for each vertex
    std::unordered_map<Vertex_Hash, glm::vec3> m_laplacians;
public:
    std::unordered_map<Vertex_Hash, glm::vec3> & get_laplacians();
    virtual void pop_vertex(Vertex *v) override;
};


class Tetrahedron : public Mesh3D {
public:
    Tetrahedron();
    virtual ~Tetrahedron() {}
};


class Pyramid : public Mesh3D {
public:
    Pyramid();
    virtual ~Pyramid() {}
};


class QueenMesh : public Mesh3D { // Mesh loaded from an OFF file
public:
    QueenMesh();
    virtual ~QueenMesh() {}
};

#endif  // MESH3D_H