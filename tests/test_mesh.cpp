#include <catch2/catch.hpp>

#include "meshes/Edge.h"
#include "meshes/Mesh2D.h"


TEST_CASE( "Check that vertex hashes are unique", "[mesh]" ) {
    std::vector<Vertex> vts;
    size_t nb_vts = 30;
    vts.reserve(nb_vts);
    for (size_t i = 0; i < nb_vts; ++i) {
        vts.push_back(Vertex());
    }
    for (size_t i = 0; i < nb_vts; ++i) {
        for (size_t j = i + 1; j < nb_vts; ++j) {
            REQUIRE(vts[i].get_hash() != vts[j].get_hash());
            REQUIRE(vts[i] != vts[j]);
        }
    }
}


TEST_CASE( "Edge basic operations", "[mesh]" ) {
    Vertex va(glm::vec3(1, 1, 0));
    Vertex vb(glm::vec3(1, 2, 0));
    Vertex vc(glm::vec3(1, 4, 0));
    Edge e1(&va, &vb);
    Edge e2(&vb, &va);
    Edge e3(&va, &vc);
    REQUIRE(
        e1.get_hash() ==    std::to_string(va.get_hash()) + "-" +
                            std::to_string(vb.get_hash())
    );
    REQUIRE(e1.get_hash() == e2.get_hash());
    REQUIRE(e1.get_length() == Approx(1.0));
    REQUIRE(e2.get_length() == Approx(1.0));
    REQUIRE(e1 < e3);
    REQUIRE(!(e3 < e1));
}


TEST_CASE( "Mesh basic operations", "[mesh]" ) {
    Mesh mesh;
    Vertex *va = mesh.add_vertex(glm::vec3(2, 7, 0));
    Vertex *vb = mesh.add_vertex(glm::vec3(5, 5, 0));
    Vertex *vc = mesh.add_vertex(glm::vec3(2, 4, 0));
    Vertex *vd = mesh.add_vertex(glm::vec3(6, 1, 0));
    Vertex *ve = mesh.add_vertex(glm::vec3(8, 4, 0));
    Vertex *vf = mesh.add_vertex(glm::vec3(6, 7, 0));
    Face *fa = mesh.add_face({ va, vc, vb });
    Face *fb = mesh.add_face({ vb, vc, vd });
    Face *fc = mesh.add_face({ vb, vd, ve });
    Face *fd = mesh.add_face({ vb, ve, vf });
    Face *fe = mesh.add_face({ va, vb, vf });
    va->set_incident_face(fa);
    vb->set_incident_face(fa);
    vc->set_incident_face(fa);
    vd->set_incident_face(fb);
    ve->set_incident_face(fc);
    vf->set_incident_face(fd);
    REQUIRE(mesh.get_nb_vertices() == 6);
    REQUIRE(mesh.get_nb_faces() == 5);

    // Check that faces are connected correctly
    mesh.connect_adjacent_faces();
    std::unordered_map<Face_Hash, std::array<Face_Hash, 3>> all_adj_faces;
    all_adj_faces[fa->get_hash()] = { fb->get_hash(), fe->get_hash(), -1 };
    all_adj_faces[fb->get_hash()] = { -1, fc->get_hash(), fa->get_hash() };
    all_adj_faces[fc->get_hash()] = { -1, fd->get_hash(), fb->get_hash() };
    all_adj_faces[fd->get_hash()] = { -1, fe->get_hash(), fc->get_hash() };
    all_adj_faces[fe->get_hash()] = { fd->get_hash(), -1, fa->get_hash() };
    int f_ind = 0;
    std::array<Face*, 3> adj_faces;
    std::array<Face_Hash, 3> expected_adj_faces;
    FaceIterator face_it;
    for (face_it = mesh.faces_begin(); face_it != mesh.faces_end(); ++face_it) {
        adj_faces = face_it->get_adjacent_faces();
        expected_adj_faces = all_adj_faces[face_it->get_hash()];
        for (size_t i = 0; i < 3; ++i) {
            if (expected_adj_faces[i] == -1) {
                REQUIRE(adj_faces[i] == nullptr);
            }
            else {
                REQUIRE(adj_faces[i]->get_hash() == expected_adj_faces[i]);
            }
        }
        ++f_ind;
    }

    // Test face and vertex circulators
    FaceCirculator f_circ = mesh.incident_faces(*vb);
    f_circ = mesh.incident_faces(*vb);
    REQUIRE(*f_circ == *fa); f_circ++;
    REQUIRE(*f_circ == *fb); ++f_circ;
    REQUIRE(*f_circ == *fc); ++f_circ;
    REQUIRE(*f_circ == *fd); ++f_circ;
    REQUIRE(*f_circ == *fe); ++f_circ;
    REQUIRE(*f_circ == *fa); ++f_circ;
    f_circ = mesh.incident_faces(*vb, *fb);
    REQUIRE(*f_circ == *fb); ++f_circ;
    REQUIRE(*f_circ == *fc); ++f_circ;
    REQUIRE(*f_circ == *fd); ++f_circ;
    REQUIRE(*f_circ == *fe); ++f_circ;
    REQUIRE(*f_circ == *fa); ++f_circ;
    REQUIRE(*f_circ == *fb); ++f_circ;
    f_circ = mesh.incident_faces(*va);
    REQUIRE(f_circ.get_nb_incident_faces() == 0);
    VertexCirculator v_circ = mesh.neighbour_vertices(*vb);
    REQUIRE(*v_circ == *va); v_circ++;
    REQUIRE(*v_circ == *vc); ++v_circ;
    REQUIRE(*v_circ == *vd); ++v_circ;
    REQUIRE(*v_circ == *ve); ++v_circ;
    REQUIRE(*v_circ == *vf); ++v_circ;
    REQUIRE(*v_circ == *va); ++v_circ;
    v_circ = mesh.neighbour_vertices(*va);
    REQUIRE(v_circ.get_nb_neighbour_vertices() == 0);

    // Pop vertices / faces
    mesh.pop_vertex(va);
    REQUIRE(mesh.get_nb_vertices() == 5);
    mesh.pop_face(fa);
    REQUIRE(mesh.get_nb_faces() == 4);
}

TEST_CASE( "Create a Mesh2D and test visibility of vertices and faces", "[mesh]" ) {
    Mesh2D mesh;
    REQUIRE(mesh.get_nb_faces() == 4);
    REQUIRE(mesh.get_nb_vertices() == 4);
    Vertex *inf_vtx = mesh.get_infinite_vertex();
    REQUIRE(inf_vtx != nullptr);
    REQUIRE(mesh.is_vertex_fictive(*inf_vtx) == true);
    VertexIterator vtx_it;
    size_t nb_vts_visible = 0;
    for (vtx_it = mesh.vertices_begin(); vtx_it != mesh.vertices_end(); ++vtx_it) {
        if (mesh.is_vertex_visible(*vtx_it))
            ++nb_vts_visible;
    }
    REQUIRE(nb_vts_visible == 0);
    FaceIterator face_it;
    size_t nb_faces_fictive = 0;
    size_t nb_faces_visible = 0;
    for (face_it = mesh.faces_begin(); face_it != mesh.faces_end(); ++face_it) {
        if (mesh.is_face_fictive(*face_it))
            ++nb_faces_fictive;
        if (mesh.is_face_visible(*face_it))
            ++nb_faces_visible;
    }
    REQUIRE(nb_faces_visible == 0);
    REQUIRE(nb_faces_fictive == 3);
}