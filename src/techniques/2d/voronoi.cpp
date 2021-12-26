#include "voronoi.h"

#include "meshes/Mesh2D.h"
#include "utils/math.h"


std::unordered_map<Face_Hash, Vertex> build_voronoi(Mesh2D *mesh) {
    std::unordered_map<Face_Hash, Vertex> voronoi_vts;
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    glm::vec3 face_center;
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        face_vts = face_it->get_vertices();
        face_center = compute_center(
            face_vts[0]->get_position(),
            face_vts[1]->get_position(),
            face_vts[2]->get_position()
        );
        voronoi_vts[face_it->get_hash()] = Vertex(face_center, &*face_it);
    }
    return voronoi_vts;
}
