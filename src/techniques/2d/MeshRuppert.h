#ifndef RUPPERT_H
#define RUPPERT_H

#include "meshes/Mesh2D.h"
#include "meshes/Edge.h"

struct PlanarStraightLineGraph {
    std::vector<glm::vec3> m_vertices;
    std::vector<std::array<int, 2>> m_edges;
};

class MeshRuppert : public Mesh2D {
protected:
    PlanarStraightLineGraph m_graph;
    std::vector<Edge> m_constraint_edges;
    BoundingBox m_constraints_bb;

public:
    MeshRuppert(const float alpha);

    bool is_constraint(const Vertex &a, const Vertex &b);

    float find_worst_aspect_ratio_triangle(float alpha, Face*& worst_triangle);

    bool is_segment_encroached_upon_by_point(
        const int edge_ind,
        const glm::vec3 p
    );
    void split_segment(const int edge_ind);
    int get_one_segment_encroached_upon();
    std::vector<int> get_segments_encroached_upon_point(
        const glm::vec3 p
    );

    void refine(float alpha);

    std::vector<Edge> get_constraint_edges();
};

#endif  // RUPPERT_H
