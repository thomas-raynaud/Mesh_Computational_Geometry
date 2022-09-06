#ifndef MESH_PARABOLA_H
#define MESH_PARABOLA_H

#include "meshes/Mesh2D.h"
#include "utils/enums.h"


// Square grid of 2D points, with Z coordinate computed with X and Y.
class MeshParabola : public Mesh2D {

private:
    ParabolaType m_parabola_type;
    float m_mesh_precision;
    // Range of the points in the X and Y axis
    float m_x_min;
    float m_x_max;
    float m_y_min;
    float m_y_max;

public:
    MeshParabola(ParabolaType parabola_type = ParabolaType::EllipticParaboloid);
    virtual ~MeshParabola() {};

    float compute_z_coordinate(const float x, const float y);

    // Weak curvature = green -> strong curvature = red
    void color_vertices();

    virtual glm::vec2 get_random_point();
};

#endif //   MESH_PARABOLA_H
