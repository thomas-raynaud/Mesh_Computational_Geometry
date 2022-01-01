#include "visibility_march.h"

#include "meshes/Mesh2D.h"
#include "techniques/predicate.h"


Face* take_step_visibility_march(
    Mesh2D *mesh,
    const Face &face,
    const glm::vec3 &p
) {
    glm::vec3 a, b;
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    std::array<Face*, 3> adj_faces = face.get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        a = face_vts[i]->get_position();
        b = face_vts[(i + 1) % 3]->get_position();
        if (test_orientation(a, b, p) < 0.f) {
            return adj_faces[(i + 2) % 3];
        }
    }
    return nullptr;
}