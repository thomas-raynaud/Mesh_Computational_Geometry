#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include <map>

#include "meshes/Mesh.h"


/**
 * Remove an edge in the mesh.
 * @ve: vertex opposite to the edge to delete, in the face fe
 * @fe: face that has the edge to delete, and the vertex ve
 */
bool collapse_edge(
    Mesh &mesh,
    Vertex *ve,
    Face *fe
);

void get_edges(
    Mesh &mesh,
    std::map<Edge_Hash, Edge> &edges,
    std::less<Edge>
);

/**
 * Simplify the mesh by removing its smallest edges. n represents the number of
 * vertices in the mesh after the simplification.
 */
void simplify(Mesh &mesh, int n);

#endif  // SIMPLIFY_H