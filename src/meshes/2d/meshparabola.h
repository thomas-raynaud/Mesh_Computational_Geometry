#ifndef MESHPARABOLA_H
#define MESHPARABOLA_H

#include "mesh2d.h"


class Parabola : public Mesh2D { // Mesh construit à partir de (x,y) |--> (x^2+y^2)
    int _parabola_type;

public:
    Parabola(int type = 0);
    virtual ~Parabola() {}
    void drawMesh();
    void drawMeshWireFrame();
};

#endif // MESHPARABOLA_H
