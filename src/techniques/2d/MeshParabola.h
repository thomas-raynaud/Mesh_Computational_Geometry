#ifndef MESH_PARABOLA_H
#define MESH_PARABOLA_H

#include "meshes/Mesh2D.h"
#include "utils/enums.h"


// Square grid of 2D points, with Z coordinate computed with X and Y.
class MeshParabola : public Mesh2D {

private:
    ParabolaType m_parabola_type;

public:
    MeshParabola(ParabolaType parabola_type = ParabolaType::EllipticParaboloid);
    virtual ~MeshParabola() {}
};

#endif //   MESH_PARABOLA_H
