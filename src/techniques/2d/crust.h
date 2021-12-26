#ifndef CRUST_H
#define CRUST_H

#include "meshes/Mesh2D.h"


class Crust : public Mesh2D {
private:
    std::unordered_map<Vertex_Hash, bool> m_is_voronoi_vertex;

public:
    Crust();
    virtual ~Crust() {}

    bool is_voronoi_vertex(const Vertex &vtx);
};

#endif  // CRUST_H
