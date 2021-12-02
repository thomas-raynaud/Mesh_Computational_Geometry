#ifndef MESH_REFINED_DELAUNAY_H
#define MESH_REFINED_DELAUNAY_H

#include "meshdelaunay.h"


class Mesh_Constrained : public Mesh2D {

public:
    Mesh_Constrained();
    virtual ~Mesh_Constrained() {}

protected:
    std::vector<Edge> constraints;
};

#endif  // MESH_REFINED_DELAUNAY_H
