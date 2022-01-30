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
    Vertex *w = mesh.add_vertex(glm::vec3(9, 11, 0));
    Vertex *x = mesh.add_vertex(glm::vec3(1, 7, 0));
    Vertex *y = mesh.add_vertex(glm::vec3(7, 1, 0));
    Vertex *z = mesh.add_vertex(glm::vec3(16, 5, 0));
    Face *f1 = mesh.add_face({ v00, v01, v11 });
    Face *f2 = mesh.add_face({ v10, v11, v01 });
    Face *f01 = mesh.add_face({ v00, v11, a });
    Face *f02 = mesh.add_face({ v00, b, v01 });
    Face *f11 = mesh.add_face({ v01, c, v10 });
    Face *f12 = mesh.add_face({ v10, d, v11 });
    Face *fa = mesh.add_face({ a, v11, w });
    Face *fb = mesh.add_face({ a, x, v00 });
    Face *fc = mesh.add_face({ b, v00, x });
    Face *fd = mesh.add_face({ b, y, v01 });
    Face *fe = mesh.add_face({ c, v01, y });
    Face *ff = mesh.add_face({ c, z, v10 });
    Face *fg = mesh.add_face({ d, v10, z });
    Face *fh = mesh.add_face({ d, w, v11 });
    v00->set_incident_face(f1);
    v01->set_incident_face(f1);
    v10->set_incident_face(f2);
    v11->set_incident_face(f2);
    a->set_incident_face(f01);
    b->set_incident_face(f02);
    c->set_incident_face(f11);
    d->set_incident_face(f12);
    w->set_incident_face(fa);
    x->set_incident_face(fb);
    y->set_incident_face(fd);
    z->set_incident_face(ff);
    mesh.connect_adjacent_faces();
    std::array<Face*, 3> adj_faces2 = f12->get_adjacent_faces();
    delaunay::flip_edge(f1, f2);
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
    REQUIRE((*adj_faces[0] == *f1 || *adj_faces[1] == *f1 || *adj_faces[2] == *f1));
    adj_faces = f12->get_adjacent_faces();
    REQUIRE((*adj_faces[0] == *f1 || *adj_faces[1] == *f1 || *adj_faces[2] == *f1));
    adj_faces = f02->get_adjacent_faces();
    REQUIRE((*adj_faces[0] == *f2 || *adj_faces[1] == *f2 || *adj_faces[2] == *f2));
    adj_faces = f11->get_adjacent_faces();
    REQUIRE((*adj_faces[0] == *f2 || *adj_faces[1] == *f2 || *adj_faces[2] == *f2));
}


TEST_CASE( "Split a triangle", "[delaunay]" ) {
    Mesh2D mesh;
    Vertex *va = mesh.add_vertex(glm::vec3(6, 11, 0));
    Vertex *vb = mesh.add_vertex(glm::vec3(8, 5, 0));
    Vertex *vc = mesh.add_vertex(glm::vec3(12, 12, 0));
    Vertex *vx = mesh.add_vertex(glm::vec3(4, 7, 0));
    Vertex *vy = mesh.add_vertex(glm::vec3(13, 7, 0));
    Vertex *vz = mesh.add_vertex(glm::vec3(8, 14, 0));
    Face *f = mesh.add_face({ va, vb, vc });
    Face *fa = mesh.add_face({ va, vx, vb });
    Face *fb = mesh.add_face({ vb, vy, vc });
    Face *fc = mesh.add_face({ vc, vz, va });
    va->set_incident_face(f);
    vb->set_incident_face(f);
    vc->set_incident_face(f);
    vx->set_incident_face(fa);
    vy->set_incident_face(fb);
    vz->set_incident_face(fc);
    mesh.connect_adjacent_faces();
    unsigned int nb_vertices = mesh.get_nb_vertices();
    unsigned int nb_faces = mesh.get_nb_faces();
    glm::vec3 p(9, 10, 0);
    Vertex *vp = delaunay::split_triangle(&mesh, p, f);
    ++nb_vertices;
    nb_faces += 2;
    REQUIRE(mesh.get_nb_vertices() == nb_vertices);
    REQUIRE(mesh.get_nb_faces() == nb_faces);
    std::array<Vertex*, 3> neighbour_vts = { va, vb, vc };
    VertexCirculator vc_p = mesh.neighbour_vertices(*vp);
    unsigned int offset = 0;
    if (vc_p->get_hash() == vb->get_hash()) offset = 1;
    else if (vc_p->get_hash() == vc->get_hash()) offset = 2;
    REQUIRE(vc_p->get_hash() == neighbour_vts[offset]->get_hash());
    vc_p++; offset = (offset + 1) % 3;
    REQUIRE(vc_p->get_hash() == neighbour_vts[offset]->get_hash());
    vc_p++; offset = (offset + 1) % 3;
    REQUIRE(vc_p->get_hash() == neighbour_vts[offset]->get_hash());
}


TEST_CASE( "Insert a vertex", "[delaunay]" ) {
    Mesh2D mesh;
    size_t nb_vertices = mesh.get_nb_vertices();
    size_t nb_faces = mesh.get_nb_faces();
    Vertex *va = delaunay::insert_vertex(&mesh, glm::vec3(0, 0, 0));
    ++nb_vertices;
    nb_faces += 2;
    REQUIRE(va != nullptr);
    REQUIRE(mesh.get_nb_vertices() == nb_vertices);
    REQUIRE(mesh.get_nb_faces() == nb_faces);
    FaceCirculator fc = mesh.incident_faces(*va);
    FaceCirculator fc_begin = fc;
    unsigned int nb_incident_faces = 0;
    do {
        ++nb_incident_faces;
        ++fc;
    } while (fc_begin != fc);
    REQUIRE(nb_incident_faces == 3);
    VertexCirculator vc = mesh.neighbour_vertices(*va);
    VertexCirculator vc_begin = vc;
    unsigned int nb_neighbour_vertices = 0;
    do {
        ++nb_neighbour_vertices;
        ++vc;
    } while (vc_begin != vc);
    REQUIRE(nb_neighbour_vertices == 3);
}