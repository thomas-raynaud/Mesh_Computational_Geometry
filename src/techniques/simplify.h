#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include <map>
#include <list>
#include <set>

#include "meshes/Mesh.h"
#include "meshes/Edge.h"


/**
 * Remove an edge in the mesh.
 */
bool collapse_edge(
    Mesh &mesh,
    std::list<Edge> &edges,
    std::set<Edge_Hash> &edge_hashes_set,
    const std::list<Edge>::iterator &edge_to_delete_it
);

// Get edges of the mesh ordered based on their length (smallest first)
void get_edges(
    Mesh &mesh,
    std::list<Edge> &edges,
    std::set<Edge_Hash> &edge_hashes_set
);

/**
 * Simplify the mesh by removing its smallest edges. n represents the number of
 * vertices in the mesh after the simplification.
 */
void simplify(Mesh &mesh, const int n);

#endif  // SIMPLIFY_H