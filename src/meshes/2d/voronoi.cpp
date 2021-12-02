#include "mesh2d.h"

Point Mesh2D::computeCenter(Point A, Point B, Point C){
       double ptanA, ptanB, ptanC, coefA, coefB, coefC;

       Point AC, AB, BA, BC, CB, CA;
       Point k(0, 0, 1);
       AC = difference(A, C);
       AB = difference(A, B);
       BA = difference(B, A);
       BC = difference(B, C);
       CB = difference(C, B);
       CA = difference(C, A);

       ptanA = dotProduct(crossProduct(AC, AB), k) * dotProduct(CB, CA) * dotProduct(BA, BC);
       ptanB = dotProduct(crossProduct(BA, BC), k) * dotProduct(CB, CA) * dotProduct(AC, AB);
       ptanC = dotProduct(crossProduct(CB, CA), k) * dotProduct(AB, AC) * dotProduct(BA, BC);

       coefA = (ptanC + ptanB);
       coefB = (ptanC + ptanA);
       coefC = (ptanA + ptanB);

       double sum = 1.0 / (coefA + coefB + coefC);

       coefA = coefA * sum;
       coefB = coefB * sum;
       coefC = coefC * sum;

       Point Q = coefA * A + coefB * B + coefC * C;
    return Q;
}

void Mesh2D::buildVoronoi(){
    int i = 0;
    _vVertices = QVector<Vertex>();
    for (QVector<Face>::iterator face_it = faceTab.begin(); face_it != faceTab.end(); ++face_it){
       Point A, B, C;
       A = m_vertices[face_it->vertices()[0]]->point();
       B = m_vertices[face_it->vertices()[1]]->point();
       C = m_vertices[face_it->vertices()[2]]->point();

       //Ajout de Q
       _vVertices.push_back(Vertex(computeCenter(A, B, C), 0, i));
       i++;
    }
}
