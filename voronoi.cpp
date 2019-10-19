#include "meshdelaunay.h"

void Mesh2D::buildVoronoi(){
    int i =4;
    _vVertices = QVector<Vertex>();
   for (QVector<Face>::iterator face_it = faceTab.begin(); face_it != faceTab.end(); ++face_it){
       Point A, B, C;
       A = vertexTab[face_it->vertices()[0]].point();
       B = vertexTab[face_it->vertices()[1]].point();
       C = vertexTab[face_it->vertices()[2]].point();

       //Calcul de Q
       double tanA, tanB, tanC, coefA, coefB, coefC;
       tanA = tangente(C, A, B);
       tanB = tangente(A, B, C);
       tanC = tangente(B, C, A);

       coefA = (tanC + tanB);
       coefB = (tanC + tanA);
       coefC = (tanA + tanB);

       double sum = 1.0 / (coefA + coefB + coefC);

       coefA = coefA * sum;
       coefB = coefB * sum;
       coefC = coefC * sum;

       Point Q = coefA * A + coefB * B + coefC * C;

       //Ajout de Q
       _vVertices.push_back(Vertex(Q, 0, i));
       i++;
   }
}
