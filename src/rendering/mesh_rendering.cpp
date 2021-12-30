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

void draw_face(const Face &face) {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    glm::vec3 color;
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < 3; ++i) {
        color = face_vts[i]->get_color();
        glColor3d(color.r, color.g, color.b);
        draw_vertex(*face_vts[i]);
    }
    glEnd();
}

void draw_face(const Face &face, const glm::vec3 &color) {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    glColor3d(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < 3; ++i) {
        glColor3d(color.r, color.g, color.b);
        draw_vertex(*face_vts[i]);
    }
    glEnd();
}

void draw_wireframe_face(const Face &face) {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    Vertex v1, v2;
    glm::vec3 c1, c2;
    for (size_t i = 0; i < 3; ++i) {
        v1 = *face_vts[i];
        v2 = *face_vts[(i + 1) % 3];
        c1 = v1.get_color();
        c2 = v2.get_color();
        glBegin(GL_LINE_STRIP);
        glColor3d(c1.r, c1.g, c1.b);
        draw_vertex(v1);
        glColor3d(c2.r, c2.g, c2.b);
        draw_vertex(v2);
        glEnd();
    }
}

void draw_wireframe_face(const Face &face, const glm::vec3 &color) {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    Vertex v1, v2;
    for (size_t i = 0; i < 3; ++i) {
        v1 = *face_vts[i];
        v2 = *face_vts[(i + 1) % 3];
        glBegin(GL_LINE_STRIP);
        glColor3d(color.r, color.g, color.b);
        draw_vertex(v1);
        glColor3d(color.r, color.g, color.b);
        draw_vertex(v2);
        glEnd();
    }
}

void draw_voronoi_wireframe(
    Mesh2D *mesh,
    std::unordered_map<Face_Hash, Vertex> &voronoi_vertices
) {
    FaceCirculator fc, fc_begin;
    VertexIterator vtx_it;
    glm::vec3 color(1, 0, 0);
    Face *f1, *f2;
    for (   vtx_it = mesh->vertices_begin();
            vtx_it != mesh->vertices_end();
            ++vtx_it
    ) {
        if (mesh->is_vertex_fictive(*vtx_it)) continue;
        fc_begin = mesh->incident_faces(*vtx_it);
        fc = fc_begin;
        do {
            f1 = &*fc;
            ++fc;
            f2 = &*fc;
            if (!mesh->is_face_visible(*f1) || !mesh->is_face_visible(*f2))
                continue;
            glBegin(GL_LINE_STRIP);
            draw_vertex(voronoi_vertices[f1->get_hash()]);
            draw_vertex(voronoi_vertices[f2->get_hash()]);
            glEnd();
        } while (fc != fc_begin);
    }
}

void Mesh::draw_mesh_vertices_colors() {
    FaceIterator face_it;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it)
        draw_face(*face_it);
}

void Mesh::draw_mesh_faces_colors() {
    FaceIterator face_it;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it)
        draw_face(*face_it, face_it->get_color());
}

void Mesh::draw_mesh_wireframe_vertices_color() {
    FaceIterator face_it;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it)
        draw_wireframe_face(*face_it);
}

void Mesh::draw_mesh_wireframe_faces_color() {
    FaceIterator face_it;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it)
        draw_wireframe_face(*face_it, face_it->get_color());
}

void Mesh2D::draw_mesh_faces_colors() {
    FaceIterator face_it;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it) {
        if (!is_face_visible(*face_it)) continue;
        draw_face(*face_it, face_it->get_color());
    }
}

void Mesh2D::draw_mesh_wireframe_faces_color() {
    FaceIterator face_it;
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it) {
        if (!is_face_visible(*face_it)) continue;
        draw_wireframe_face(*face_it, face_it->get_color());
    }
}

void MeshCrust::draw_mesh_wireframe_faces_color() {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    Vertex *v1, *v2;
    glColor3d(0, 1, 0);
    for (face_it = faces_begin(); face_it != faces_end(); ++face_it) {
        if (!is_face_visible(*face_it)) continue;
        face_vts = face_it->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            v1 = face_vts[i];
            v2 = face_vts[(i + 1) % 3];
            if (    !is_voronoi_vertex(*v1) &&
                    !is_voronoi_vertex(*v2)
            ) {
                glBegin(GL_LINE_STRIP);
                draw_vertex(*v1);
                draw_vertex(*v2);
                glEnd();
            }
        }
    }
}

void MeshRuppert::draw_mesh_wireframe_faces_color() {
    FaceIterator face_it;
    std::array<Vertex*, 3> face_vts;
    Vertex *v1, *v2;
    for (face_it = faces_begin();face_it != faces_end(); ++face_it) {
        if (!is_face_visible(*face_it)) continue;
        face_vts = face_it->get_vertices();
        for (size_t i = 0; i < 3; ++i) {
            v1 = face_vts[i];
            v2 = face_vts[(i + 1) % 3];
            if (is_constraint(*v1, *v2))
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