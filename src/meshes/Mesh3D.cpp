#include "Mesh3D.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "utils/read_3d_file.h"


Mesh3D::Mesh3D(const std::string filename) {
    Data3D mesh_data;
    read_3d_file(filename, &mesh_data);
    // Vertices
    std::vector<Vertex*> vts;
    std::vector<Face*> faces;
    std::array<float, 4> vtx_data;
    std::array<int, 3> face_data;
    for (size_t vtx_ind = 0; vtx_ind < mesh_data.vertices.size(); ++vtx_ind) {
        vtx_data = mesh_data.vertices[vtx_ind];
        vts.push_back(add_vertex(glm::vec3(vtx_data[0], vtx_data[1], vtx_data[2])));
    }
    // Faces
    for (size_t face_ind = 0; face_ind < mesh_data.faces.size(); ++face_ind) {
        face_data = mesh_data.faces[face_ind];
        faces.push_back(add_face({ vts[face_data[0]], vts[face_data[1]], vts[face_data[2]] }));
        // Link faces to vertices
        for (size_t i = 0; i < 3; ++i) {
            vts[face_data[i]]->set_incident_face(faces[faces.size() - 1]);
        }
    }
    connect_adjacent_faces();
}

std::unordered_map<Vertex_Hash, glm::vec3> & Mesh3D::get_laplacians() {
    return m_laplacians;
}


void Mesh3D::pop_vertex(Vertex *v) {
    m_laplacians.erase(v->get_hash());
    Mesh::pop_vertex(v);
}