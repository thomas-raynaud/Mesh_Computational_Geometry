#include "mesh_rendering.h"

#include "meshes/Mesh.h"
#include "meshes/Mesh2D.h"
#include "techniques/2d/MeshCrust.h"
#include "techniques/2d/MeshRuppert.h"

#include <QGLWidget>


void draw_vertex(const Vertex &vtx) {
    glm::vec3 pos = vtx.get_position();
    glVertex3f(pos.x, pos.y, pos.z);
}

void draw_face(const Face &face, const glm::vec3 *color) {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    glm::vec3 color_vt;
    if (color != nullptr)
        glColor3d(color->r, color->g, color->b);
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < 3; ++i) {
        if (color == nullptr) {
            color_vt = face_vts[i]->get_color();
            glColor3d(color_vt.r, color_vt.g, color_vt.b);
        }
        draw_vertex(*face_vts[i]);
    }
    glEnd();
}

void draw_wireframe_face(const Face &face, const glm::vec3 *color) {
    if (color != nullptr)
        glColor3d(color->r, color->g, color->b);
    glm::vec3 c1, c2;
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    Vertex v1, v2;
    for (size_t i = 0; i < 3; ++i) {
        v1 = *face_vts[i];
        v2 = *face_vts[(i + 1) % 3];
        if (color == nullptr) {
            c1 = v1.get_color();
            c2 = v2.get_color();
        }
        glBegin(GL_LINE_STRIP);
        if (color == nullptr)
            glColor3d(c1.r, c1.g, c1.b);
        draw_vertex(v1);
        if (color == nullptr)
            glColor3d(c2.r, c2.g, c2.b);
        draw_vertex(v2);
        glEnd();
    }
}

void draw_voronoi_wireframe(
    Mesh2D *mesh,
    std::unordered_map<Face_Hash, glm::vec3> &voronoi_pts
) {
    FaceCirculator fc, fc_begin;
    VertexIterator vtx_it;
    glm::vec3 color(1, 0, 0);
    glm::vec3 p;
    Face *f1, *f2;
    glColor3d(0.9, 0.1, 0.6);
    for (   vtx_it = mesh->vertices_begin();
            vtx_it != mesh->vertices_end();
            ++vtx_it
    ) {
        if (!mesh->is_vertex_visible(*vtx_it)) continue;
        fc_begin = mesh->incident_faces(*vtx_it);
        fc = fc_begin;
        do {
            f1 = &*fc;
            ++fc;
            f2 = &*fc;
            if (!mesh->is_face_visible(*f1) || !mesh->is_face_visible(*f2)) {
                continue;
            }
            glBegin(GL_LINE_STRIP);
            p = voronoi_pts[f1->get_hash()];
            glVertex3f(p.x, p.y, p.z);
            p = voronoi_pts[f2->get_hash()];
            glVertex3f(p.x, p.y, p.z);
            glEnd();
        } while (fc != fc_begin);
    }
}

void draw_mesh(Mesh *mesh, bool use_vertices_colors) {
    FaceIterator face_it;
    glm::vec3 face_color;
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        face_color = face_it->get_color();
        draw_face(*face_it, (use_vertices_colors ? nullptr : &face_color));
    }
}

void draw_mesh_wireframe(Mesh *mesh, bool use_vertices_colors) {
    FaceIterator face_it;
    glm::vec3 face_color;
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        face_color = face_it->get_color();
        draw_wireframe_face(*face_it, (use_vertices_colors ? nullptr : &face_color));
    }
}

void draw_mesh(Mesh2D *mesh, bool use_vertices_colors) {
    FaceIterator face_it;
    glm::vec3 face_color;
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        if (!mesh->is_face_visible(*face_it)) continue;
        face_color = face_it->get_color();
        draw_face(*face_it, (use_vertices_colors ? nullptr : &face_color));
    }
}

void draw_mesh(MeshCrust *mesh, bool use_vertices_colors) {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    Vertex *v1, *v2;
    glm::vec3 face_color;
    bool skip_face;
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        if (!mesh->is_face_visible(*face_it)) continue;
        skip_face = false;
        face_vts = face_it->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            if (mesh->is_voronoi_vertex(*face_vts[i])) {
                skip_face = true;
                break;
            }
        }
        if (skip_face) continue;
        face_color = face_it->get_color();
        draw_wireframe_face(*face_it, (use_vertices_colors ? nullptr : &face_color));
    }
}

void draw_mesh_wireframe(
    Mesh2D *mesh,
    bool use_vertices_colors,
    bool show_voronoi
) {
    FaceIterator face_it;
    glm::vec3 color = glm::vec3(0, 1, 0);
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        if (!mesh->is_face_visible(*face_it)) continue;
        if (!show_voronoi)
            color = face_it->get_color();
        draw_wireframe_face(*face_it, (use_vertices_colors ? nullptr : &color));
    }
}

void draw_mesh_wireframe(MeshCrust *mesh) {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    Vertex *v1, *v2;
    glColor3d(0, 1, 0);
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        if (!mesh->is_face_visible(*face_it)) continue;
        face_vts = face_it->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            v1 = face_vts[i];
            v2 = face_vts[(i + 1) % 3];
            if (    !mesh->is_voronoi_vertex(*v1) &&
                    !mesh->is_voronoi_vertex(*v2)
            ) {
                glBegin(GL_LINE_STRIP);
                draw_vertex(*v1);
                draw_vertex(*v2);
                glEnd();
            }
        }
    }
}

void draw_mesh_wireframe(MeshRuppert *mesh) {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    Vertex *v1, *v2;
    for (face_it = mesh->faces_begin(); face_it != mesh->faces_end(); ++face_it) {
        if (!mesh->is_face_visible(*face_it)) continue;
        face_vts = face_it->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            v1 = face_vts[i];
            v2 = face_vts[(i + 1) % 3];
            if (mesh->is_constraint(*v1, *v2))
                glColor3d(1, 0, 0);
            else
                glColor3d(0, 1, 0);
            glBegin(GL_LINE_STRIP);
            draw_vertex(*v1);
            draw_vertex(*v2);
            glEnd();
        }
    }
}