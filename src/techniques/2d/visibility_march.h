#ifndef VISIBILITY_MARCH_H
#define VISIBILITY_MARCH_H

#include "meshes/Face.h"
#include "meshes/Vertex.h"


class Mesh2D;

// Returns the next face where we need to go to insert vtx. If vtx is in face,
// we return a nullptr.
Face* take_step_visibility_march(
    Mesh2D *mesh,
    const Face &face,
    const Vertex &vtx
);

#endif  // VISIBILITY_MARCH