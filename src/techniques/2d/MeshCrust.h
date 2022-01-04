#ifndef MESH_CRUST_H
#define MESH_CRUST_H

#include "meshes/Mesh2D.h"


class MeshCrust : public Mesh2D {
private:
    std::unordered_map<Vertex_Hash, bool> m_is_voronoi_vertex;

public:
    MeshCrust();
    virtual ~MeshCrust() {}

    bool is_voronoi_vertex(const Vertex &vtx);
};

#endif  // MESH_CRUST_H
