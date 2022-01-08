#include "visibility_march.h"

#include "meshes/Mesh2D.h"
#include "techniques/predicate.h"


Face* take_step_visibility_march(
    Mesh2D *mesh,
    const Face &face,
    const glm::vec3 &p
) {
    glm::vec3 a, b, c;
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    std::array<Face*, 3> adj_faces = face.get_adjacent_faces();
    a = face_vts[0]->get_position();
    b = face_vts[1]->get_position();
    c = face_vts[2]->get_position();
    bool orientation_face = test_orientation(a, b, c) > 0.f;
    bool orientation_p;
    for (size_t i = 0; i < 3; ++i) {
        a = face_vts[i]->get_position();
        b = face_vts[(i + 1) % 3]->get_position();
        orientation_p = test_orientation(a, b, p) > 0.f;
        if (orientation_face != orientation_p) {
            return adj_faces[(i + 2) % 3];
        }
    }
    return nullptr;
}