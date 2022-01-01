#include "MeshRuppert.h"

#include "delaunay.h"
#include "utils/math.h"


MeshRuppert::MeshRuppert() : Mesh2D() {
    // Vertices
    Vertex * v1  = insert_delaunay_vertex(this, glm::vec3( -1, -1, 0));
    Vertex * v2  = insert_delaunay_vertex(this, glm::vec3(  1, -1, 0));
    Vertex * v3  = insert_delaunay_vertex(this, glm::vec3(  0,  1, 0));
    Vertex * v4  = insert_delaunay_vertex(this, glm::vec3(  3,  3, 0));
    Vertex * v5  = insert_delaunay_vertex(this, glm::vec3( -2,  4, 0));
    Vertex * v6  = insert_delaunay_vertex(this, glm::vec3(  1,  8, 0));
    Vertex * v7  = insert_delaunay_vertex(this, glm::vec3( -1,  9, 0));
    Vertex * v8  = insert_delaunay_vertex(this, glm::vec3( -7, 12, 0));
    Vertex * v9  = insert_delaunay_vertex(this, glm::vec3(-10,  5, 0));
    Vertex * v10 = insert_delaunay_vertex(this, glm::vec3( -4,  3, 0));
    Vertex * v11 = insert_delaunay_vertex(this, glm::vec3( -4,  2, 0));
    Vertex * v12 = insert_delaunay_vertex(this, glm::vec3( -7,  1, 0));
    Vertex * v13 = insert_delaunay_vertex(this, glm::vec3( -3, -1, 0));
    Vertex * v14 = insert_delaunay_vertex(this, glm::vec3(  2,  7, 0));
    // Constraints
    std::vector<Edge> edge_constraints_vec = {
        Edge(v4, v14),
        Edge(v14, v3),
        Edge(v3, v5),
        Edge(v5, v6),
        Edge(v6, v7),
        Edge(v7, v8),
        Edge(v8, v9),
        Edge(v9, v10),
        Edge(v10, v11),
        Edge(v11, v12),
        Edge(v12, v13),
        Edge(v13, v1),
        Edge(v1, v2),
        Edge(v2, v4),
    };
    Edge *e;
    for (size_t i = 0; i < edge_constraints_vec.size(); ++i) {
        e = &edge_constraints_vec[i];
        m_edge_constraints[e->get_hash()] = *e;
    }
}


bool MeshRuppert::is_constraint(Vertex &a, Vertex &b){
    Edge e(&a, &b);
    return (m_edge_constraints.find(e.get_hash()) != m_edge_constraints.end());
}


std::vector<Edge_Hash> MeshRuppert::constraint_edges_encroached_upon() {
    std::vector<Edge_Hash> encroached_edges;
    std::unordered_map<Edge_Hash, Edge>::iterator edge_it;
    VertexCirculator vc, vc_begin;
    bool edge_exists;
    for (
            edge_it = m_edge_constraints.begin();
            edge_it != m_edge_constraints.end();
            ++edge_it
    ) {
        vc_begin = neighbour_vertices(*(edge_it->second.v1));
        vc = vc_begin;
        edge_exists = false;
        do {
            edge_exists = *(edge_it->second.v2) == *vc;
            ++vc;
        } while (vc != vc_begin && !edge_exists);
        if (!edge_exists)
            encroached_edges.push_back(edge_it->first);
    }
    return encroached_edges;
}


void MeshRuppert::split_edge(const Edge_Hash &edge_hash) {
    Edge e = m_edge_constraints[edge_hash];
    glm::vec3 a = e.v1->get_position();
    glm::vec3 b = e.v2->get_position();
    Vertex *new_vtx = insert_delaunay_vertex(this, (a + b) * 0.5f);
    m_edge_constraints.erase(edge_hash);
    Edge e1(e.v1, new_vtx);
    Edge e2(new_vtx, e.v2);
    m_edge_constraints[e1.get_hash()] = e1;
    m_edge_constraints[e2.get_hash()] = e2;
}


void MeshRuppert::split_encroached_constraint_edges() {
    std::vector<Edge_Hash> encroached_edges = constraint_edges_encroached_upon();
    while (encroached_edges.size()) {
        split_edge(encroached_edges[0]);
        encroached_edges = constraint_edges_encroached_upon();
    }
}


Face* MeshRuppert::find_worst_aspect_ratio_triangle(float alpha) {
    Face *worst_triangle = nullptr;
    float angle_max = -1.f;
    float angle_max_face;
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    glm::vec3 a, b, c;
    float cos_bac, cos_abc, cos_bca;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it) {
        if (!is_face_visible(*face_it))
            continue;
        angle_max_face = face_it->get_max_angle();
        if (angle_max_face > alpha && angle_max_face > angle_max) {
            angle_max = angle_max_face;
            worst_triangle = &*face_it;
        }
    }
    return worst_triangle;
}


void MeshRuppert::refine(float alpha) {
    split_encroached_constraint_edges();
    Face *face = find_worst_aspect_ratio_triangle(alpha);
    std::array<Vertex*, 3> face_vts;
    glm::vec3 a, b, c, q;
    std::vector<Edge_Hash> encroached_edges;
    while (face != nullptr) {
        face_vts = face->get_vertices();
        a = face_vts[0]->get_position();
        b = face_vts[1]->get_position();
        c = face_vts[2]->get_position();
        // Insert the Voronoi center in a temp mesh
        q = compute_center(a, b, c);
        MeshRuppert mesh_tmp = *this;
        insert_delaunay_vertex(&mesh_tmp, q);
        encroached_edges = mesh_tmp.constraint_edges_encroached_upon();
        if (encroached_edges.size() == 0) {
            insert_delaunay_vertex(this, q);
        }
        else {
            // Split the constraint edges that q encroaches upon
            for (   size_t edge_ind = 0;
                    edge_ind < encroached_edges.size();
                    ++edge_ind
            ) {
                split_edge(encroached_edges[edge_ind]);
            }
        }
    }
}