#ifndef PARABOLA_H
#define PARABOLA_H

#include "meshes/Mesh2D.h"


enum ParabolaType {
    EllipticParaboloid,
    Rosenbrock
};

// Square grid of 2D points, with Z coordinate computed with X and Y.
class Parabola : public Mesh2D {

private:
    ParabolaType m_parabola_type;

public:
    Parabola(ParabolaType parabola_type = ParabolaType::EllipticParaboloid);
    virtual ~Parabola() {}
};

#endif //   PARABOLA_H
