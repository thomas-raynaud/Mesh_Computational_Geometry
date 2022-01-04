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

void draw_face(const Face &face);
void draw_face(const Face &face, const glm::vec3 &color);
void draw_wireframe_face(const Face &face);
void draw_wireframe_face(const Face &face, const glm::vec3 &color);

void draw_voronoi_wireframe(
    Mesh2D *mesh,
    std::unordered_map<Face_Hash, glm::vec3> &voronoi_pts
);

void draw_mesh_vertices_colors(Mesh *mesh);
void draw_mesh_faces_colors(Mesh *mesh);
void draw_mesh_wireframe_vertices_color(Mesh *mesh);
void draw_mesh_wireframe_faces_color(Mesh *mesh);
void draw_mesh_faces_colors(Mesh2D *mesh);
void draw_mesh_wireframe_faces_color(Mesh2D *mesh);
void draw_mesh_wireframe_faces_color(MeshCrust *mesh);
void draw_mesh_wireframe_faces_color(MeshRuppert *mesh);

#endif  // MESH_RENDERING_H
