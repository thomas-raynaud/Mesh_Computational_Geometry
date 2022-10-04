#include "MeshRuppert.h"

#include "delaunay.h"
#include "utils/math.h"
#include "utils/read_obj.h"

#include <iostream>


MeshRuppert::MeshRuppert(const float alpha) {
    ObjData ruppert_data;
    std::string filename = "resources/ruppert.obj";
    // Read OBJ file
    if (read_obj(filename, &ruppert_data) != 0) {
        std::cerr << "Error: could not read obj file at " << filename << std::endl;
        return;
    }
    // Fill the graph the OBJ structure
    std::array<float, 4> vt_data;
    for (size_t i = 0; i < ruppert_data.vertices.size(); ++i) {
        vt_data = ruppert_data.vertices[i];
        m_graph.m_vertices.push_back(
            glm::vec3(vt_data[0], vt_data[1], vt_data[2])
        );
    }
    int vtx_ind, vtx_ind2;
    for (size_t i = 0; i < ruppert_data.polylines.size(); ++i) {
        vtx_ind = ruppert_data.polylines[i][0] - 1;
        for (size_t j = 0; j < ruppert_data.polylines[i].size(); ++j) {
            vtx_ind = ruppert_data.polylines[i][j] - 1;
            vtx_ind2 = ruppert_data.polylines[i][(j + 1) % ruppert_data.polylines[i].size()] - 1;
            m_graph.m_edges.push_back({ vtx_ind, vtx_ind2 });
            vtx_ind = vtx_ind2;
        }
    }
    // Refine
    refine(alpha);
}

bool MeshRuppert::is_constraint(const Vertex &a, const Vertex &b) {
    Edge *e;
    Vertex_Hash a_hash = a.get_hash();
    Vertex_Hash b_hash = b.get_hash();
    for (size_t i = 0; i < m_constraint_edges.size(); ++i) {
        e = &m_constraint_edges[i];
        if (    (e->v1->get_hash() == a_hash && e->v2->get_hash() == b_hash)
            ||  (e->v1->get_hash() == b_hash && e->v2->get_hash() == a_hash)) {
            return true;
        }
    }
    return false;
}


float MeshRuppert::find_worst_aspect_ratio_triangle(float alpha, Face*& worst_triangle) {
    worst_triangle = nullptr;
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
    if (angle_max == -1.f)
        return -1.f;
    else
        return std::acos(angle_max);
}


void MeshRuppert::split_segment(const int edge_ind) {
    Edge e = m_constraint_edges[edge_ind];
    glm::vec3 a = e.v1->get_position();
    glm::vec3 b = e.v2->get_position();
    Vertex *new_vtx = delaunay::insert_vertex(*this, (a + b) * 0.5f);
    m_constraint_edges[edge_ind] = Edge(e.v1, new_vtx);
    m_constraint_edges.push_back(Edge(new_vtx, e.v2));
    int last_ind = m_constraint_edges.size() - 1;
}

bool MeshRuppert::is_segment_encroached_upon_by_point(
    const int edge_ind,
    const glm::vec3 p
) {
    const glm::vec3 v1 = m_constraint_edges[edge_ind].v1->get_position();
    const glm::vec3 v2 = m_constraint_edges[edge_ind].v2->get_position();
    return is_point_in_diametral_circle_of_segment(p, v1, v2);
}

int MeshRuppert::get_one_segment_encroached_upon() {
    Edge *e;
    for (size_t i = 0; i < m_constraint_edges.size(); ++i) {
        e = &m_constraint_edges[i];
        for (VertexIteratorType vtx_it = m_vertices.begin(); vtx_it != m_vertices.end(); ++vtx_it) {
            if (!is_vertex_visible(vtx_it->second))
                continue;
            if (vtx_it->first == e->v1->get_hash() || vtx_it->first == e->v2->get_hash())
                continue;
            if (are_points_close(e->v1->get_position(), e->v2->get_position()))
                continue;
            if (is_segment_encroached_upon_by_point(i, vtx_it->second.get_position())) {
                glm::vec3 p = vtx_it->second.get_position();
                glm::vec3 s1 = e->v1->get_position();
                glm::vec3 s2 = e->v2->get_position();
                return i;
            }
        }
    }
    // No constraint edges encroached upon
    return -1;
}

std::vector<int> MeshRuppert::get_segments_encroached_upon_point(
    const glm::vec3 p
) {
    Edge *e;
    std::vector<int> encroached_segments;
    for (size_t i = 0; i < m_constraint_edges.size(); ++i) {
        if (is_segment_encroached_upon_by_point(i, p))
            encroached_segments.push_back(i);
    }
    return encroached_segments;
}

void MeshRuppert::refine(const float alpha) {
    float alpha_rad = glm::radians(alpha);
    float smallest_angle;
    int segment_encroached_upon;
    std::vector<int> segments_encroached_upon_p;
    Face *worst_triangle;
    // Compute initial Delaunay triangulation
    m_faces.clear();
    m_vertices.clear();
    m_constraint_edges.clear();
    create_mesh_wrapper();
    std::vector<Vertex*> inserted_vts;
    Vertex* inserted_vtx;
    std::array<int, 2> constraint_edge;
    glm::vec3 p;
    for (size_t i = 0; i < m_graph.m_vertices.size(); ++i) {
        inserted_vtx = delaunay::insert_vertex(*this, m_graph.m_vertices[i]);
        inserted_vts.push_back(inserted_vtx);
    }
    for (size_t i = 0; i < m_graph.m_edges.size(); ++i) {
        constraint_edge = m_graph.m_edges[i];
        m_constraint_edges.push_back(
            Edge(inserted_vts[constraint_edge[0]], inserted_vts[constraint_edge[1]])
        );
    }
    segment_encroached_upon = get_one_segment_encroached_upon();
    smallest_angle = find_worst_aspect_ratio_triangle(alpha_rad, worst_triangle);

    while (segment_encroached_upon != -1 || (worst_triangle != nullptr && smallest_angle < alpha_rad)) {
        while (segment_encroached_upon != -1) {
            split_segment(segment_encroached_upon);
            segment_encroached_upon = get_one_segment_encroached_upon();
        }
        smallest_angle = find_worst_aspect_ratio_triangle(alpha_rad, worst_triangle);
        if (worst_triangle == nullptr)
            continue;
        p = compute_circumcenter(
            worst_triangle->get_vertices()[0]->get_position(),
            worst_triangle->get_vertices()[1]->get_position(),
            worst_triangle->get_vertices()[2]->get_position()
        );
        segments_encroached_upon_p = get_segments_encroached_upon_point(p);
        if (segments_encroached_upon_p.size() > 0) {
            for (size_t i = 0; i < segments_encroached_upon_p.size(); ++i) {
                split_segment(segments_encroached_upon_p[i]);
            }
        }
        else {
            delaunay::insert_vertex(*this, p);
        }
        segment_encroached_upon = get_one_segment_encroached_upon();
        smallest_angle = find_worst_aspect_ratio_triangle(alpha_rad, worst_triangle);
    }
}

std::vector<Edge> MeshRuppert::get_constraint_edges() {
    return m_constraint_edges;
}