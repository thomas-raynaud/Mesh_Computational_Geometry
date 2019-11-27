#ifndef MESH_RUPPERT_H
#define MESH_RUPPERT_H
#include "meshdelaunay.h"

class MeshRuppert : public Mesh2D{
protected:
    QVector<std::array<int, 2>> _constraint; //Arrete de contrainte

public:
    MeshRuppert();

    QVector<std::array<int, 2>> constraint(){return _constraint;}

    virtual void drawMesh();
    bool isConstraint(int a, int b);

    void priority();
    QVector<int> edgeNotInDel();

};

#endif // MESH_RUPPERT_H
