#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <glm/glm.hpp>


class Mesh2D;
class Face;
class Vertex;

namespace delaunay {
    // Flip the edge between f1 and f2. If the edge cannot be flipped, return false.
    void flip_edge(Face *f1, Face *f2);
    // Split a triangle in three triangles, with the new point inserted inside
    Vertex* split_triangle(Mesh2D *mesh, glm::vec3 &point, Face *face);
    void rearrange_around_vertex(Mesh2D *mesh, Vertex *vtx);
    Vertex* insert_vertex(Mesh2D *mesh, glm::vec3 p);
}

#endif  // DELAUNAY_H