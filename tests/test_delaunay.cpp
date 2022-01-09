#include <catch2/catch.hpp>

#include "meshes/Mesh2D.h"
#include "techniques/2d/delaunay.h"


TEST_CASE( "Flip an edge", "[delaunay]" ) {
    Mesh2D mesh;
    Vertex *v00 = mesh.add_vertex(glm::vec3(4, 6, 0));
    Vertex *v01 = mesh.add_vertex(glm::vec3(7, 3, 0));
    Vertex *v10 = mesh.add_vertex(glm::vec3(12, 5, 0));
    Vertex *v11 = mesh.add_vertex(glm::vec3(8, 8, 0));
    Vertex *a = mesh.add_vertex(glm::vec3(3, 10, 0));
    Vertex *b = mesh.add_vertex(glm::vec3(4, 4, 0));
    Vertex *c = mesh.add_vertex(glm::vec3(11, 2, 0));
    Vertex *d = mesh.add_vertex(glm::vec3(14, 9, 0));
    Face *f1 = mesh.add_face({ v00, v01, v11 });
    Face *f2 = mesh.add_face({ v10, v11, v01 });
    Face *f01 = mesh.add_face({ v00, v11, a });
    Face *f02 = mesh.add_face({ v00, b, v01 });
    Face *f11 = mesh.add_face({ v01, c, v10 });
    Face *f12 = mesh.add_face({ v10, d, v11 });
    v00->set_incident_face(f1);
    v01->set_incident_face(f1);
    v10->set_incident_face(f2);
    v11->set_incident_face(f2);
    a->set_incident_face(f01);
    b->set_incident_face(f02);
    c->set_incident_face(f11);
    d->set_incident_face(f12);
    mesh.connect_adjacent_faces();
    flip_edge(f1, f2);
    // Check vertices's incident faces
    Face *inc_face = v00->get_incident_face();
    REQUIRE((
        *inc_face == *f1 || *inc_face == *f2 ||
        *inc_face == *f01 || *inc_face == *f02
    ));
    inc_face = v10->get_incident_face();
    REQUIRE((
        *inc_face == *f1 || *inc_face == *f2 ||
        *inc_face == *f11 || *inc_face == *f12
    ));
    inc_face = v01->get_incident_face();
    REQUIRE((*inc_face == *f2 || *inc_face == *f02 || *inc_face == *f11 ));
    inc_face = v11->get_incident_face();
    REQUIRE((*inc_face == *f1 || *inc_face == *f01 || *inc_face == *f12 ));
    // Check vertices and adjacent faces of f1
    std::array<Vertex*, 3> face_vts = f1->get_vertices();
    std::array<Face*, 3> adj_faces = f1->get_adjacent_faces();
    size_t count_a = 0, count_b = 0, count_c = 0;
    for (size_t i = 0; i < 3; ++i) {
        if (*face_vts[i] == *v00) ++count_a;
        if (*face_vts[i] == *v10) ++count_b;
        if (*face_vts[i] == *v11) ++count_c;
    }
    REQUIRE((count_a == 1 && count_b == 1 && count_c == 1));
    count_a = 0; count_b = 0; count_c = 0;
    for (size_t i = 0; i < 3; ++i) {
        if (*adj_faces[i] == *f12) ++count_a;
        if (*adj_faces[i] == *f01) ++count_b;
        if (*adj_faces[i] == *f2) ++count_c;
    }
    REQUIRE((count_a == 1 && count_b == 1 && count_c == 1));
    // Check vertices and adjacent faces of f2
    face_vts = f2->get_vertices();
    adj_faces = f2->get_adjacent_faces();
    count_a = 0, count_b = 0, count_c = 0;
    for (size_t i = 0; i < 3; ++i) {
        if (*face_vts[i] == *v00) ++count_a;
        if (*face_vts[i] == *v01) ++count_b;
        if (*face_vts[i] == *v10) ++count_c;
    }
    REQUIRE((count_a == 1 && count_b == 1 && count_c == 1));
    count_a = 0; count_b = 0; count_c = 0;
    for (size_t i = 0; i < 3; ++i) {
        if (*adj_faces[i] == *f11) ++count_a;
        if (*adj_faces[i] == *f1) ++count_b;
        if (*adj_faces[i] == *f02) ++count_c;
    }
    REQUIRE((count_a == 1 && count_b == 1 && count_c == 1));
    // Check adjacent faces
    adj_faces = f01->get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        if (adj_faces[i] != nullptr) REQUIRE(*adj_faces[i] == *f1);
    }
    adj_faces = f12->get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        if (adj_faces[i] != nullptr) REQUIRE(*adj_faces[i] == *f1);
    }
    adj_faces = f02->get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        if (adj_faces[i] != nullptr) REQUIRE(*adj_faces[i] == *f2);
    }
    adj_faces = f11->get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        if (adj_faces[i] != nullptr) REQUIRE(*adj_faces[i] == *f2);
    }
}

TEST_CASE( "Split a triangle", "[delaunay]" ) {

}

/*
TEST_CASE( "Insert a vertex", "[delaunay]" ) {
    Mesh2D mesh;
    size_t nb_vertices = mesh.get_nb_vertices();
    Vertex *va = insert_delaunay_vertex(&mesh, glm::vec3(0, 0, 0));
    ++nb_vertices;
    REQUIRE(va != nullptr);
    REQUIRE(mesh.get_nb_vertices() == nb_vertices);
}*/