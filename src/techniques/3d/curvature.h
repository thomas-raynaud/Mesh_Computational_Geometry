#ifndef CURVATURE_H
#define CURVATURE_H

#include "utils/enums.h"


class Mesh3D;

void compute_laplacians(Mesh3D *mesh);
void set_curvature_colors(Mesh3D *mesh, ColorDisplayType color_display_type);

#endif  // CURVATURE_H