#ifndef ENUMS_H
#define ENUMS_H


enum Dimension {
    D2,
    D3
};

enum MeshDisplayType {
    PlainFaces,
    Wireframe
};

enum Algorithm2DType {
    Delaunay,
    Crust,
    Ruppert,
    Parabola
};

enum Mesh3DType {
    Tetrahedron,
    Pyramid,
    QueenStatue
};

enum ColorDisplayType {
    MeanCurvature,
    XCurvature,
    YCurvature,
    ZCurvature
};

enum ParabolaType {
    EllipticParaboloid,
    Rosenbrock
};

#endif  // ENUMS_H