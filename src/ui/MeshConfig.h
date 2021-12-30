#ifndef MESH_CONFIG_H
#define MESH_CONFIG_H

#include "utils/enums.h"


struct MeshConfig {
    Dimension dimension;
    MeshDisplayType mesh_display_type;
    Algorithm2DType algorithm_2d_type;
    bool show_voronoi_display;
    ParabolaType parabola_type;
    ColorDisplayType color_display_type;
    Mesh3DType mesh_3d_type;
};

#endif  // MESH_CONFIG_H