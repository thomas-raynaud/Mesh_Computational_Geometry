#include "simplify.h"

#include <set>

#include "meshes/Edge.h"


bool collapse_edge(
        Mesh &mesh,
        Vertex *ve,
        Face *fe
    ) {
    Vertex *v1, *v2;    // [v1-v2] is the edge to delete. v1-v2-ve corresponds to fe.
    Face *fe_opp;       // Face opposite to ve from the face fe.
    Face *fa1, *fa2;    // Adjacent faces to fe. fa1 & fa2 != fe_opp
    Face *fa3, *fa4;   // Adjacent faces to fe_opp. fa3 & fa4 != fe

    FaceCirculator fc; // Circulate on the faces of v1 and v2

    // Find v1, v2, fa1, fa2 and fe_opp
    std::array<Vertex *, 3> face_vts = fe->get_vertices();
    std::array<Face *, 3> adj_faces = fe->get_adjacent_faces();
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (*face_vts[v_ind] == *ve) {
            fe_opp = adj_faces[v_ind];
            v1 = face_vts[(v_ind + 1) % 3];
            v2 = face_vts[(v_ind + 2) % 3];
            fa1 = adj_faces[(v_ind + 1) % 3];
            fa2 = adj_faces[(v_ind + 2) % 3];
            break;
        }
    }
    // Find fa3 and fa4
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (face_vts[v_ind] == v1) {
            fa3 = adj_faces[v_ind];
            fa4 = adj_faces[(v_ind + 2) % 3];
            break;
        }
    }

    // Check if fa1 / fa2 and fa3 / fa4 are not already connected.
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (
            *(fa1->get_adjacent_faces()[v_ind]) == *fa2 ||
            *(fa3->get_adjacent_faces()[v_ind]) == *fa4)
        {
            return false;
        }
    }
    fc = mesh.incident_faces(*v1, *fe);
    ++fc;
    while (*fc != *fe_opp) {
        face_vts = fc->get_vertices();
        for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
            // Replace v1 with v2 for the adjacent faces of v1
            if (face_vts[v_ind] == v1) {
                fc->set_vertex(v_ind, v2);
            }
            // Change the face attached by each vertex
            face_vts[v_ind]->set_incident_face(&*fc);
        }
        ++fc;
    }

    // Connect fa1 <-> fa2, fa3 <-> fa4
    for (size_t f_ind = 0; f_ind < 3; ++f_ind) {
        // fa1 <-> fa2
        if (*(fa1->get_adjacent_faces()[f_ind]) == *fe)
            fa1->set_adjacent_face(f_ind, fa2);
        if (*(fa2->get_adjacent_faces()[f_ind]) == *fe)
            fa2->set_adjacent_face(f_ind, fa1);
        // fa3 <-> fa4
        if (*(fa3->get_adjacent_faces()[f_ind]) == *fe_opp)
            fa3->set_adjacent_face(f_ind, fa4);
        if (*(fa4->get_adjacent_faces()[f_ind]) == *fe_opp)
            fa4->set_adjacent_face(f_ind, fa3);
    }

    // Change the coordinates of v2. Place it in the middle of the deleted edge.
    v2->set_position((v1->get_position() + v2->get_position()) * 0.5f);

    mesh.pop_vertex(v1);
    mesh.pop_face(fe);
    mesh.pop_face(fe_opp);

    return true;
}


void get_edges(Mesh &mesh, std::vector<Edge> &edges) {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    std::set<Edge_Hash> edge_hashes_set;
    for(face_it = mesh.faces_begin(); face_it != mesh.faces_end(); ++face_it) {
        face_vts = face_it->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            Edge e(
                face_vts[(i + 1) % 3],
                face_vts[(i + 2) % 3],
                &*face_it,
                face_vts[i]
            );
            if (edge_hashes_set.count(e.get_hash()) == 0) {
                edges.push_back(e);
                edge_hashes_set.insert(e.get_hash());
            }
        }
    }
}


void simplify(Mesh &mesh, int n) {
    // Don't simplify if the number of vertices is <= to n.
    if (mesh.get_nb_vertices() <= n) return;

    // Edges sorted based on their length
    std::vector<Edge> edges;
    int nb_vertices = mesh.get_nb_vertices();
    bool edge_deleted;
    Edge *edge_to_delete;
    std::vector<Edge>::iterator edge_it;
    do {
        get_edges(mesh, edges);
        // Delete the smallest one.
        edge_it = edges.begin();
        edge_deleted = false;

        while (!edge_deleted) {
            edge_to_delete = &*edge_it;
            edge_deleted = collapse_edge(
                mesh,
                edge_to_delete->ve,
                edge_to_delete->fe
            );
            ++edge_it;
        }
    } while (n < nb_vertices && edges.size() > 0 && edge_deleted);
}