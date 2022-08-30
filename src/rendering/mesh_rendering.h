#ifndef MESH_RENDERING_H
#define MESH_RENDERING_H

#include <unordered_map>

#include "meshes/Vertex.h"
#include "meshes/Face.h"

class Mesh;
class Mesh2D;
class MeshCrust;
class MeshRuppert;


void draw_vertex(const Vertex &vtx);

// Draw a plain face. If the color of the face is not defined, color each
// vertex of the face with its own color.
void draw_face(const Face &face, const glm::vec3 *color=nullptr);
// Draw a face in wireframe. If the color of the face is not defined, color each
// vertex of the face with its own color.
void draw_wireframe_face(const Face &face, const glm::vec3 *color=nullptr);

void draw_voronoi_wireframe(
    Mesh2D *mesh,
    std::unordered_map<Face_Hash, glm::vec3> &voronoi_pts
);

void draw_mesh(Mesh *mesh, bool use_vertices_colors=false);
void draw_mesh_wireframe(Mesh *mesh, bool use_vertices_colors=false);
void draw_mesh(Mesh2D *mesh, bool use_vertices_colors=false);
void draw_mesh(MeshCrust *mesh, bool use_vertices_colors=false);
void draw_mesh_wireframe(
    Mesh2D *mesh,
    bool use_vertices_colors=false,
    bool show_voronoi=false
);
void draw_mesh_wireframe(MeshCrust *mesh);
void draw_mesh_wireframe(MeshRuppert *mesh);

#endif  // MESH_RENDERING_H
