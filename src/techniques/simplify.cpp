#include "simplify.h"

#include <algorithm>

#include "meshes/Edge.h"


bool collapse_edge(
        Mesh &mesh,
        std::list<Edge> &edges,
        std::set<Edge_Hash> &edge_hashes_set,
        const std::list<Edge>::iterator &edge_to_delete_it
    ) {
    Vertex *ve;         // Vertex opposite to the edge to delete, in the face fe
    Face *fe;           // face that has the edge to delete, and the vertex ve
    Vertex *v1, *v2;    // [v1-v2] is the edge to delete. fe = v1-v2-ve
    Vertex *ve_opp;     // fe_opp = ve_opp-v2-v1
    Face *fe_opp;       // Face opposite to ve from the face fe.
    Face *fa1, *fa2;    // Adjacent faces to fe. fa1 & fa2 != fe_opp
    Face *fa3, *fa4;    // Adjacent faces to fe_opp. fa3 & fa4 != fe
    Vertex *va1, *va3;  // va1-ve-v2 = fa1, va3-v2-ve_opp = fa3
    Vertex *va2, *va4;  // ve-va2-v1 = fa2, v1-va4-ve_opp = fa4

    // Goal of this function: delete edge v1-v2.
    // v1 + fe + fe_opp will be removed.

    FaceCirculator fc; // Circulate on the faces of v1 and v2

    ve = edge_to_delete_it->ve;
    fe = edge_to_delete_it->fe;

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
    face_vts = fe_opp->get_vertices();
    adj_faces = fe_opp->get_adjacent_faces();
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (face_vts[v_ind] == v1) {
            fa3 = adj_faces[v_ind];
            fa4 = adj_faces[(v_ind + 2) % 3];
            ve_opp = face_vts[(v_ind + 1) % 3];
            break;
        }
    }

    // Find va1, va2, va3, va4
    face_vts = fa1->get_vertices();
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (face_vts[v_ind] != ve && face_vts[v_ind] != v2) {
            va1 = face_vts[v_ind];
        }
    }
    face_vts = fa2->get_vertices();
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (face_vts[v_ind] != v1 && face_vts[v_ind] != ve) {
            va2 = face_vts[v_ind];
        }
    }
    face_vts = fa3->get_vertices();
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (face_vts[v_ind] != ve_opp && face_vts[v_ind] != v2) {
            va3 = face_vts[v_ind];
        }
    }
    face_vts = fa4->get_vertices();
    for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
        if (face_vts[v_ind] != ve_opp && face_vts[v_ind] != v1) {
            va4 = face_vts[v_ind];
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
    // Update incident faces of v1
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
    // Update incident faces of v2
    fc = mesh.incident_faces(*v2, *fe_opp);
    ++fc;
    while (*fc != *fe) {
        face_vts = fc->get_vertices();
        for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
            // Change the face attached by each vertex
            face_vts[v_ind]->set_incident_face(&*fc);
        }
        ++fc;
    }
    // Update incident faces of ve
    fc = mesh.incident_faces(*ve, *fe);
    ++fc;
    while (*fc != *fe) {
        face_vts = fc->get_vertices();
        for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
            // Change the face attached by each vertex
            face_vts[v_ind]->set_incident_face(&*fc);
        }
        ++fc;
    }
    // Update incident faces of ve_opp
    fc = mesh.incident_faces(*ve_opp, *fe_opp);
    ++fc;
    while (*fc != *fe_opp) {
        face_vts = fc->get_vertices();
        for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
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

    // Update the array of edges : delete and re-insert edges with their new
	// properties. Edges that have no change of attributes but are attached to
	// v2 also need to be inserted back, as their length have changed.
	std::vector<Edge_Hash> special_cases = {
		// These edges won't be re-inserted in the edges list.
		// Some will be re-inserted, but with their properties changed. They are
		// in the new_edges array.
		Edge(v1, va4	).get_hash(),
		Edge(v1, ve_opp	).get_hash(),
		Edge(v2, ve_opp	).get_hash(),
		Edge(v2, va3	).get_hash(),
		Edge(v2, va1	).get_hash(),
		Edge(v2, ve		).get_hash(),
		Edge(v1, ve		).get_hash(),
		Edge(v1, va2	).get_hash(),
		Edge(v1, v2		).get_hash()
	};
    // New edges
	std::vector<Edge> new_edges = {
		Edge(v2, va4,		fa4, ve_opp),
		Edge(v2, ve_opp,	fa3, va3),
		Edge(v2, va3,		fa3, ve_opp),
		Edge(v2, va1,		fa1, ve),
		Edge(v2, ve,		fa1, va1),
		Edge(v2, va2,		fa2, ve)
	};
    std::list<Edge>::iterator edge_it;
    for (edge_it = edges.begin(); edge_it != edges.end(); ++edge_it) {
        if (	edge_it->v1 == v1 || edge_it->v2 == v1
			|| 	edge_it->v1 == v2 || edge_it->v2 == v2) {
			bool is_edge_in_special_cases = false;
			for (Edge_Hash special_case : special_cases) {
				if (edge_it->get_hash() == special_case) {
					is_edge_in_special_cases = true;
					break;
				}
			}
			if (!is_edge_in_special_cases) {
				if (edge_it->v1 == v1)
					edge_it->v1 = v2;
				else if (edge_it->v2 == v1)
					edge_it->v2 = v2;
				new_edges.push_back(*edge_it);
			}
			edge_it = edges.erase(edge_it);
        }
        else if (edge_it->ve == v1) {
            edge_it->ve = v2;
        }
    }
    // Re-insert edges based on their length
	for (Edge e : new_edges) {
		edges.insert(std::lower_bound(edges.begin(), edges.end(), e), e);
	}

    mesh.pop_vertex(v1);
    mesh.pop_face(fe);
    mesh.pop_face(fe_opp);

    return true;
}


void get_edges(
    Mesh &mesh,
    std::list<Edge> &edges,
    std::set<Edge_Hash> &edge_hashes_set
    ) {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    edges.clear();
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
    edges.sort();
}


#include <iostream>
void simplify(Mesh &mesh, const int n) {
    // Don't simplify if the number of vertices is <= to n.
    if (mesh.get_nb_vertices() <= n) return;
    std::cout << "nb vertices : " << mesh.get_nb_vertices() << std::endl;
    int n_tmp = n;
    //n_tmp = mesh.get_nb_vertices() - 10;

    // Edges sorted based on their length
    std::list<Edge> edges;
    std::set<Edge_Hash> edge_hashes_set;
    get_edges(mesh, edges, edge_hashes_set);
    std::list<Edge>::iterator edge_it;
    bool edge_deleted;
    do {
        //timer.start();
        //get_edges(mesh, edges, edge_hashes_set);
        //time_get_edges += timer.elapsed();
        // Delete the smallest one.
        edge_it = edges.begin();
        edge_deleted = false;

        while (!edge_deleted && edge_it != edges.end()) {
            edge_deleted = collapse_edge(
                mesh,
                edges,
                edge_hashes_set,
                edge_it
            );
            ++edge_it;
        }
		std::cout << n << " ->" << mesh.get_nb_vertices() << std::endl;
    } while (n_tmp <= mesh.get_nb_vertices() && edges.size() > 0 && edge_deleted);
    std::cout << "nb vertices end : " << mesh.get_nb_vertices() << std::endl;
    std::cout << "----------" << std::endl;
}