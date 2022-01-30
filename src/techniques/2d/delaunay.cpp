#include "delaunay.h"

#include <queue>

#include "meshes/Mesh2D.h"
#include "techniques/predicate.h"
#include "techniques/2d/visibility_march.h"


namespace delaunay {

void flip_edge(Face *f1, Face *f2) {
    Vertex *v00, *v01;              // Vertices of f1 with v00 opposite of f2
    Vertex *v10, *v11;              // Vertices of f2 with v10 opposite of f1
    Face *f01, *f02, *f11, *f12;    // Faces opposite of v01, v02, v11, v12 respectively
    int ind_v00 = 0, ind_v10 = 0;
    for (size_t i = 0; i < 3; ++i) {
        if (*(f1->get_adjacent_faces()[i]) == *f2) ind_v00 = i;
        if (*(f2->get_adjacent_faces()[i]) == *f1) ind_v10 = i;
    }
    // f1 vertices
    v00 = f1->get_vertices()[ind_v00];
    v01 = f1->get_vertices()[(ind_v00 + 1) % 3];
    // f2 vertices
    v10 = f2->get_vertices()[ind_v10];
    v11 = f2->get_vertices()[(ind_v10 + 1) % 3];
    // Opposite faces
    f01 = f1->get_adjacent_faces()[(ind_v00 + 1) % 3];
    f02 = f1->get_adjacent_faces()[(ind_v00 + 2) % 3];
    f11 = f2->get_adjacent_faces()[(ind_v10 + 1) % 3];
    f12 = f2->get_adjacent_faces()[(ind_v10 + 2) % 3];

    // Change the vertices and adjacent faces of f1 and f2
    f1->set_vertices({ v00, v10, v11 });
    f2->set_vertices({ v00, v01, v10 });
    f1->set_adjacent_faces({ f12, f01,  f2 });
    f2->set_adjacent_faces({ f11,  f1, f02 });
    // Change the vertices' incident faces
    v00->set_incident_face(f1);
    v01->set_incident_face(f2);
    v10->set_incident_face(f2);
    v11->set_incident_face(f1);

    // Change f02 & f12's adjacent faces
    std::array<Face*, 3> adj_faces = f02->get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        if (*adj_faces[i] == *f1) {
            f02->set_adjacent_face(i, f2);
            break;
        }
    }
    adj_faces = f12->get_adjacent_faces();
    for (size_t i = 0; i < 3; ++i) {
        if (*adj_faces[i] == *f2) {
            f12->set_adjacent_face(i, f1);
            break;
        }
    }
}


Vertex* split_triangle(Mesh2D *mesh, glm::vec3 &point, Face *face) {
    std::array<Vertex*, 3> face_vts = face->get_vertices();
    std::array<Face*, 3> adj_faces = face->get_adjacent_faces();
    Vertex *vtx = mesh->add_vertex(point);
    // Create 2 new faces
    Face *fa = mesh->add_face({ vtx, face_vts[0], face_vts[1] });
    Face *fb = mesh->add_face({ vtx, face_vts[1], face_vts[2] });
    fa->set_adjacent_faces({ adj_faces[2], fb, face });
    fb->set_adjacent_faces({ adj_faces[0], face, fa });
    // Update the face split
    face->set_vertices({ vtx, face_vts[2], face_vts[0] });
    face->set_adjacent_faces({ adj_faces[1], fa, fb });
    // Update fa's adjacent face.
    std::array<Vertex*, 3> adj_face_vts = adj_faces[2]->get_vertices();
    for (size_t i = 0; i < 3; ++i) {
        if (*adj_face_vts[i] != *face_vts[0] &&
            *adj_face_vts[i] != *face_vts[1]
        ) {
            adj_faces[2]->set_adjacent_face(i, fa);
            break;
        }
    }
    // Update fb's adjacent face.
    adj_face_vts = adj_faces[0]->get_vertices();
    for (size_t i = 0; i < 3; ++i) {
        if (*adj_face_vts[i] != *face_vts[1] &&
            *adj_face_vts[i] != *face_vts[2]
        ) {
            adj_faces[2]->set_adjacent_face(i, fb);
            break;
        }
    }
    // Update vertices' incident faces.
    face_vts[0]->set_incident_face(fa);
    face_vts[1]->set_incident_face(fb);
    face_vts[2]->set_incident_face(face);
    vtx->set_incident_face(fa);
    return vtx;
}


void rearrange_around_vertex(Mesh2D *mesh, Vertex *vtx) {
    // Queue containing the triangles to test. The edge to test is opposite to
    // vtx in each triangle.
    std::queue<Face*> face_queue;
    glm::vec3 a, b, c, d;
    a = vtx->get_position();
    FaceCirculator fc_begin = mesh->incident_faces(*vtx);
    FaceCirculator fc = fc_begin;
    Face *f1, *f2;
    std::array<Vertex*, 3> face_vts;
    std::array<Face*, 3> adj_faces;
    // Add to the queue the triangles around vtx
    do {
        face_queue.push(&*fc);
        ++fc;
    } while (fc != fc_begin);
    
    while (!face_queue.empty()) {
        f1 = face_queue.front();
        face_queue.pop();
        if (mesh->is_face_fictive(*f1)) continue;
        // Test if locally Delaunay : check if d is in the circle of the
        // triangle abc (f1).
        face_vts = f1->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            if (*(face_vts[i]) == *vtx) {
                // bc = applicant edge for flip
                b = face_vts[(i + 1) % 3]->get_position();
                c = face_vts[(i + 1) % 3]->get_position();
                // f2 = triangle opposite to a
                f2 = f1->get_adjacent_faces()[i];
                break;
            }
        }
        if (mesh->is_face_fictive(*f2)) continue;
        // d = vertex in f2 opposite to f1
        face_vts = f2->get_vertices();
        adj_faces = f2->get_adjacent_faces();
        for (size_t i = 0; i < 3; ++i) {
            if (*(adj_faces[i]) == *f1) {
                d = face_vts[i]->get_position();
            }
        }
        if (is_in_circle(a, b, c, d)) { // Triangle not Delaunay locally
            flip_edge(f1, f2);
            // Add the newly formed edges
            face_queue.push(f1);
            face_queue.push(f2);
        }
    }
}


Vertex* insert_vertex(Mesh2D *mesh, glm::vec3 p) {
    // Check if the point is inside the convex hull.
    glm::vec3 a, b, c;
    Vertex *new_vtx = nullptr;
    Vertex *inf_vtx = mesh->get_infinite_vertex();
    FaceCirculator fc_begin = mesh->incident_faces(*inf_vtx);
    FaceCirculator fc = fc_begin;
    std::array<Vertex*, 3> face_vts;
    Face *face_split = nullptr;
    Face *face_start = nullptr;
    Face *previous_face;
    do {
        face_vts = fc->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            if (    *(face_vts[(i + 2) % 3]) != *inf_vtx &&
                    *(face_vts[i]) != *inf_vtx
            ) {
                a = face_vts[(i + 2) % 3]->get_position();
                b = face_vts[i]->get_position();
            }
        }
        if (test_orientation(a, b, p) > 0.f) // Edge visible from p
            face_split = &*fc;
        ++fc;
    } while (fc != fc_begin && face_split == nullptr);

    if (face_split != nullptr) {
        new_vtx = split_triangle(mesh, p, face_split);
        // Flip fictive edges if they are incident to another fictive face.
        fc_begin = mesh->incident_faces(*inf_vtx);
        fc = fc_begin;
        do {
            face_vts = fc->get_vertices();
            for (size_t i = 0; i < 3; ++i) {
                if (    *(face_vts[i]) == *new_vtx &&
                        *(face_vts[i + 1]) == *inf_vtx
                ) {
                    face_start = &*fc;
                    break;
                }
            }
            if (face_start != nullptr) ++fc;
        } while (fc != fc_begin && face_start == nullptr);
        // Add visible edges after the split face
        bool edge_visible = true;
        previous_face = &*(++fc);
        ++fc;
        while (edge_visible) {
            face_vts = fc->get_vertices();
            for (size_t i = 0; i < 3; ++i) {
                if (    *(face_vts[(i + 2) % 3]) != *inf_vtx &&
                        *(face_vts[i]) != *inf_vtx
                ) {
                    a = face_vts[(i + 2) % 3]->get_position();
                    b = face_vts[i]->get_position();
                }
            }
            if (test_orientation(a, b, p) > 0.f) { // Edge visible from p
                flip_edge(&*fc, previous_face);
                fc = mesh->incident_faces(*inf_vtx, *previous_face);
                ++fc;
            }
            else edge_visible = false;
        }
        // Add visible edges before the split face
        fc = mesh->incident_faces(*inf_vtx, *face_start);
        previous_face = &*fc;
        --fc;
        edge_visible = true;
        while(edge_visible) {
            face_vts = fc->get_vertices();
            for (size_t i = 0; i < 3; ++i) {
                if (    *(face_vts)[(i + 2) % 3] != *inf_vtx &&
                        *(face_vts[i]) != *inf_vtx
                ) {
                    a = face_vts[(i + 2) % 3]->get_position();
                    b = face_vts[i]->get_position();
                }
            }
            if (test_orientation(a, b, p) > 0.f) { // Edge visible from p
                flip_edge(previous_face, &*fc);
                previous_face = &*fc;
                fc = mesh->incident_faces(*inf_vtx, *previous_face);
                --fc;
            }
            else edge_visible = false;
        }
        return new_vtx;
    }

    // The point is in the convex hull. Find in which face it is located.
    // -> visibility march
    // We start from a random face.
    FaceIterator face_it = mesh->faces_begin();
    size_t rand_face_ind = rand() % mesh->get_nb_faces();
    for (size_t i = 0; i < rand_face_ind; ++i) {
        ++face_it;
    }
    //std::advance(face_it, rand() % mesh->get_nb_faces());
    while (mesh->is_face_fictive(*face_it)) ++face_it;
    Face *face = &*face_it;
    do {
        previous_face = face;
        face = take_step_visibility_march(mesh, *face, p);
    } while (face != nullptr); // We arrived in the correct triangle
    new_vtx = split_triangle(mesh, p, previous_face);
    rearrange_around_vertex(mesh, new_vtx);
    return new_vtx;
}

}