#ifndef MESHCRUST_H
#define MESHCRUST_H

#include <fstream>
#include <sstream>
#include <string>
#include "meshdelaunay.h"

class Crust : public Mesh2D{
private:
    int _firstVoronoiIndex;

public:
    Crust();
    virtual ~Crust() {}

    void drawMeshWireFrame();

};

#endif // MESHCRUST_H
