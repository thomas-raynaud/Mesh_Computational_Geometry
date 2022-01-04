#ifndef RUPPERT_H
#define RUPPERT_H

#include "meshes/Mesh2D.h"
#include "meshes/Edge.h"


class MeshRuppert : public Mesh2D {
protected:
    std::unordered_map<Edge_Hash, Edge> m_edge_constraints;

public:
    MeshRuppert();

    bool is_constraint(Vertex &a, Vertex &b);
    // Returns the constraints that do not exist as edges of the mesh.
    std::vector<Edge_Hash> constraint_edges_encroached_upon();
    void split_edge(const Edge_Hash &edge_hash);
    // Split constraint edges in half until all of them are part of the Delaunay
    // triangulation.
    void split_encroached_constraint_edges();
    Face* find_worst_aspect_ratio_triangle(float alpha);
    void refine(float alpha);
};

#endif  // RUPPERT_H
