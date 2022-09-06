#include "MeshParabola.h"

#include <cmath>

#include "utils/math.h"
#include "utils/color.h"
#include "techniques/2d/delaunay.h"

#define ROSENBROCK_A 1.f
#define ROSENBROCK_B 100.f


MeshParabola::MeshParabola(ParabolaType parabola_type)
        : Mesh2D(), m_parabola_type(parabola_type)
{
    glm::vec2 p;
    float z;
    m_x_min = -1.f;
    m_x_max = 1.f;
    m_y_min = -1.f;
    m_y_max = 1.f;
    m_mesh_precision = 2000;
    // Simulate the function
    for(size_t i = 0; i < m_mesh_precision; ++i) {
        p = get_random_point();
        z = compute_z_coordinate(p.x, p.y);
        delaunay::insert_vertex(*this, glm::vec3(p, z));
    }
    color_vertices();
}

float MeshParabola::compute_z_coordinate(const float x, const float y) {
    float z;
    if (m_parabola_type == ParabolaType::EllipticParaboloid)
        z = pow(x, 2) + pow(y, 2);
    else if (m_parabola_type == ParabolaType::Rosenbrock) {
        z = pow(ROSENBROCK_A - x, 2.0) + ROSENBROCK_B * pow(y - x * x, 2.0);
        z = z / 150.f;
    }
    return z;
}

void MeshParabola::color_vertices() {
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
        hue = ((vtx_it->get_position().z - zmin) / zmax) * 270.f + 90.f;
        vtx_it->set_color(hsv2rgb(hue, 1.f, 1.f));
    }
}

glm::vec2 MeshParabola::get_random_point() {
    return glm::vec2(
        get_random_value(m_x_min, m_x_max),
        get_random_value(m_y_min, m_y_max)
    );
}