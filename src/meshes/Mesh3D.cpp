#include "Mesh3D.h"

#include <fstream>
#include <sstream>
#include <iostream>


Tetrahedron::Tetrahedron() {
    // Vertices
    Vertex *va = add_vertex(glm::vec3(-0.5,   -0.5,   -0.5));
    Vertex *vb = add_vertex(glm::vec3( 0.5,   -0.5,   -0.5));
    Vertex *vc = add_vertex(glm::vec3( 0,      0.5,   -0.5));
    Vertex *vd = add_vertex(glm::vec3( 0,     -0.5,    0.5));
    // Faces
    Face *fa = add_face({ va, vb, vc });
    Face *fb = add_face({ vb, vd, vc });
    Face *fc = add_face({ vd, va, vc });
    Face *fd = add_face({ va, vd, vb });
    // Link faces to vertices
    va->set_incident_face(fa);
    vb->set_incident_face(fb);
    vc->set_incident_face(fc);
    vd->set_incident_face(fd);
    connect_adjacent_faces();
}

Pyramid::Pyramid() {
    // Vertices
    Vertex *va = add_vertex(glm::vec3(-0.5,   -0.5,   -0.5));
    Vertex *vb = add_vertex(glm::vec3(-0.5,    0.5,   -0.5));
    Vertex *vc = add_vertex(glm::vec3( 0.5,   -0.5,   -0.5));
    Vertex *vd = add_vertex(glm::vec3( 0.5,    0.5,   -0.5));
    Vertex *ve = add_vertex(glm::vec3( 0  ,    0  ,    0.5));
    // Faces
    Face *fa = add_face({ va, vb, vc });
    Face *fb = add_face({ vb, vd, vc });
    Face *fc = add_face({ va, ve, vb });
    Face *fd = add_face({ vb, ve, vd });
    Face *fe = add_face({ vc, vd, ve });
    Face *ff = add_face({ va, vc, ve });
    // Link faces to vertices
    va->set_incident_face(fa);
    vb->set_incident_face(fa);
    vc->set_incident_face(fa);
    vd->set_incident_face(fb);
    vd->set_incident_face(fc);
    connect_adjacent_faces();
}

MeshQueen::MeshQueen() {
    // Read file
    std::ifstream off_file("resources/queen.off");
    if (!off_file) {
        std::cerr << "Cannot open queen.off file." << std::endl;
        return;
    }
    std::string line;
    int nb_vertices, nb_faces;
    std::getline(off_file, line);
    std::istringstream iss(line);
    iss >> nb_vertices >> nb_faces;

    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    // Create vertices
    float x, y, z;
    int delimiter_pos1, delimiter_pos2, delimiter_pos3;
    for (int i = 0; i < nb_vertices; ++i) {
        std::getline(off_file, line);
        delimiter_pos1 = line.find(" ", 0);
        x = atof(line.substr(0, delimiter_pos1).c_str());
        delimiter_pos2 = line.find(" ", delimiter_pos1 + 1);
        y = atof(line.substr(delimiter_pos1, delimiter_pos2).c_str());
        delimiter_pos3 = line.find(" ", delimiter_pos2 + 1);
        z = atof(line.substr(delimiter_pos2, delimiter_pos3 ).c_str());
        vertices.push_back(Vertex(glm::vec3(x, y, z)));
    }

    // Create faces
    int nb_verts_face, v1, v2, v3;
    std::array<int, 3> ind_vertices;
    for (size_t face_ind = 0; face_ind < nb_faces; ++face_ind) {
        std::getline(off_file, line);
        iss.str(line);
        iss >> nb_verts_face >> v1 >> v2 >> v3;
        iss.clear();
        ind_vertices = { v1, v2, v3 };
        Face f({ &vertices[v1], &vertices[v2], &vertices[v3] });
        faces.push_back(f);

        vertices[v1].set_incident_face(&f);
        vertices[v2].set_incident_face(&f);
        vertices[v3].set_incident_face(&f);
    }

    for (Vertex v : vertices) { m_vertices.insert({ v.get_hash(), v }); }
    for (Face f : faces) { m_faces.insert({ f.get_hash(), f }); }

    connect_adjacent_faces();
}


std::unordered_map<Vertex_Hash, glm::vec3> & Mesh3D::get_laplacians() {
    return m_laplacians;
}


void Mesh3D::pop_vertex(Vertex *v) {
    m_laplacians.erase(v->get_hash());
    Mesh::pop_vertex(v);
}