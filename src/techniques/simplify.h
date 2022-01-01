#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include <map>

#include "meshes/Mesh.h"
#include "meshes/Edge.h"


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

// Get edges of the mesh ordered based on their length (smallest first)
void get_edges(Mesh &mesh, std::vector<Edge> &edges);

/**
 * Simplify the mesh by removing its smallest edges. n represents the number of
 * vertices in the mesh after the simplification.
 */
void simplify(Mesh &mesh, int n);

#endif  // SIMPLIFY_H