#include "MeshParabola.h"

#include <cmath>

#include "utils/math.h"
#include "utils/color.h"
#include "techniques/2d/delaunay.h"

#define MESH_PRECISION 2000


MeshParabola::MeshParabola(ParabolaType parabola_type)
        : Mesh2D(), m_parabola_type(parabola_type)
{
    float x, y, z;

    // Simulate the function
    for(size_t i = 0; i < MESH_PRECISION; ++i) {
        x = -1.f + (((float) rand()) / (float) RAND_MAX) * 2.f;
        y = -1.f + (((float) rand()) / (float) RAND_MAX) * 2.f;

        if (parabola_type == ParabolaType::EllipticParaboloid)
            z = pow(x, 2) + pow(y, 2);
        else if (parabola_type == ParabolaType::Rosenbrock) {
            float a = 1.f;
            float b = 100.f;
            z = pow(a - x, 2.0) + b * pow(y - x * x, 2.0);
            z = z / 150.f;
        }
        delaunay::insert_vertex(*this, glm::vec3(x, y, z));
    }
    // Color attribution
    // Find Z min and Z max
    float zmin = FLT_MAX, zmax = 0.f, hue;
    VertexIterator vtx_it;
    for (vtx_it = vertices_begin(); vtx_it != vertices_end(); ++vtx_it) {
        if (!is_vertex_visible(*vtx_it)) continue;
        zmin = std::min(zmin, vtx_it->get_position().z);
        zmax = std::max(zmax, vtx_it->get_position().z);
    }
    for (vtx_it = vertices_begin(); vtx_it != vertices_end(); ++vtx_it) {
        // Weak curvature = green -> strong curvature = red
        hue = ((vtx_it->get_position().z - zmin) / zmax) * 270.f + 90.f;
        vtx_it->set_color(hsv2rgb(hue, 1.f, 1.f));
    }
}
