#include <catch2/catch.hpp>

#include "meshes/Mesh2D.h"
#include "techniques/2d/delaunay.h"

/*
TEST_CASE( "Insert a vertex", "[delaunay]" ) {
    Mesh2D mesh;
    size_t nb_vertices = mesh.get_nb_vertices();
    Vertex *va = insert_delaunay_vertex(&mesh, glm::vec3(0, 0, 0));
    ++nb_vertices;
    REQUIRE(va != nullptr);
    REQUIRE(mesh.get_nb_vertices() == nb_vertices);
}*/