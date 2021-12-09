#ifndef MESH_RUPPERT_H
#define MESH_RUPPERT_H

#include "mesh2d.h"


class MeshRuppert : public Mesh2D{
protected:
    QVector<std::array<int, 2>> _constraint; //Arrete de contrainte

public:
    MeshRuppert();

    QVector<std::array<int, 2>> constraint(){return _constraint;}

    virtual void drawMesh() override;
    bool isConstraint(int a, int b);

    void splitEdge(int);

    void priority();
    QVector<int> edgeNotInDel();

    double cos(Point a, Point b, Point c);
    int findWorstTriangle(double alpha);

    void raffinement(double alpha);

};

#endif  // MESH_RUPPERT_H
