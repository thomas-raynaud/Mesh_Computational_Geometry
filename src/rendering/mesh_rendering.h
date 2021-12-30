#ifndef MESH_RENDERING_H
#define MESH_RENDERING_H

#include "meshes/Vertex.h"
#include "meshes/Face.h"

class Mesh2D;


void draw_vertex(const Vertex &vtx);

void draw_face(const Face &face);
void draw_face(const Face &face, const glm::vec3 &color);
void draw_wireframe_face(const Face &face);
void draw_wireframe_face(const Face &face, const glm::vec3 &color);

void draw_voronoi_wireframe(
    Mesh2D *mesh,
    std::unordered_map<Face_Hash, Vertex> &voronoi_vertices
);

#endif  // MESH_RENDERING_H