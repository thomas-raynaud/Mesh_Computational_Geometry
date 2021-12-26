#ifndef CURVATURE_H
#define CURVATURE_H


class Mesh3D;

void compute_laplacians(Mesh3D *mesh);

void set_curvature_colors(Mesh3D *mesh, int curvature_axis);

#endif  // CURVATURE_H