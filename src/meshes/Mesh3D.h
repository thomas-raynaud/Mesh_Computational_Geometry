#ifndef MESH3D_H
#define MESH3D_H

#include "meshes/Mesh.h"


class Mesh3D : public Mesh {
protected:
    // Stores a Laplacian for each vertex
    std::unordered_map<Vertex_Hash, glm::vec3> m_laplacians;
public:
    Mesh3D(const std::string filename);
    virtual ~Mesh3D() {}
    std::unordered_map<Vertex_Hash, glm::vec3> & get_laplacians();
    virtual void pop_vertex(Vertex *v) override;
};

#endif  // MESH3D_H