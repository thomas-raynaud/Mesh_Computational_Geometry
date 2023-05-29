#ifndef ENUMS_H
#define ENUMS_H


enum struct Dimension {
    D2,
    D3
};

enum struct MeshDisplayType {
    PlainFaces,
    Wireframe
};

enum struct Algorithm2DType {
    Delaunay,
    Crust,
    Ruppert,
    Parabola
};

enum struct Mesh3DType {
    Tetrahedron,
    Pyramid,
    QueenStatue,
    Monkey
};

enum struct ColorDisplayType {
    MeanCurvature,
    XCurvature,
    YCurvature,
    ZCurvature
};

enum struct ParabolaType {
    EllipticParaboloid,
    Rosenbrock
};

#endif  // ENUMS_H