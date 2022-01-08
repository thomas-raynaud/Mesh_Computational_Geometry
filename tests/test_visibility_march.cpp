#include <catch2/catch.hpp>

#include "techniques/2d/visibility_march.h"
#include "meshes/Mesh2D.h"


TEST_CASE( "Check visibility march", "[visibility_march]" ) {
    glm::vec3 a(2, 3, 0);
    glm::vec3 b(4, 6, 0);
    glm::vec3 c(4, 3, 0);
    glm::vec3 d(2, 7, 0);
    glm::vec3 e(7, 8, 0);
    glm::vec3 f(9, 5, 0);
    glm::vec3 p(8, 11, 0);
    Mesh2D mesh;
    Vertex *va = mesh.add_vertex(a);
    Vertex *vb = mesh.add_vertex(b);
    Vertex *vc = mesh.add_vertex(c);
    Vertex *vd = mesh.add_vertex(d);
    Vertex *ve = mesh.add_vertex(e);
    Vertex *vf = mesh.add_vertex(f);
    Face *fa = mesh.add_face({ va, vc, vb });
    Face *fb = mesh.add_face({ vb, vd, va });
    Face *fc = mesh.add_face({ vc, ve, vb });
    Face *fd = mesh.add_face({ vc, vf, ve });
    Face *fe = mesh.add_face({ vb, ve, vd });
    va->set_incident_face(fa);
    vb->set_incident_face(fa);
    vc->set_incident_face(fa);
    vd->set_incident_face(fb);
    ve->set_incident_face(fc);
    vf->set_incident_face(fd);
    mesh.connect_adjacent_faces();
    Face *curr_face = fa;
    curr_face = take_step_visibility_march(&mesh, *curr_face, p);
    REQUIRE(curr_face != nullptr);
    REQUIRE(*curr_face == *fc);
    curr_face = take_step_visibility_march(&mesh, *curr_face, p);
    REQUIRE(curr_face != nullptr);
    REQUIRE(*curr_face == *fe);
    curr_face = take_step_visibility_march(&mesh, *curr_face, p);
    REQUIRE(curr_face == nullptr);
}