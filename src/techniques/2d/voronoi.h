#ifndef VORONOI_H
#define VORONOI_H

#include <unordered_map>

#include "meshes/Face.h"


class Mesh2D;

std::unordered_map<Face_Hash, Vertex> build_voronoi(Mesh2D *mesh);

#endif  // VORONOI_H