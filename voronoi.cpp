#include "mesh.h"

void Mesh2D::buildVoronoi(){
   for (int faceIndex = 0; faceIndex < this->faceTab.size(); faceIndex++){

       Point A, B, C;
       A = this->vertexTab[this->faceTab[faceIndex].vertices()[0]].point();
       B = this->vertexTab[this->faceTab[faceIndex].vertices()[1]].point();
       C = this->vertexTab[this->faceTab[faceIndex].vertices()[2]].point();

       //Calcul de Q
       double tanA, tanB, tanC, coefA, coefB, coefC;
       somme(A, B);
       tanA = tan(B, A, C);
       tanB = tan(A, B, C);
       tanC = tan(A, C, B);

       coefA = 0.5*(tanC + tanB);
       coefB = 0.5*(tanC + tanA);
       coefC = 0.5*(tanA + tanB);
       double sum = 1 / (coefA + coefB + coefC);
       coefA = coefA * sum;
       coefB = coefB * sum;

       Point Q;
       Q = somme(somme(scalarProduct(coefA, A), scalarProduct(coefB, B)), scalarProduct(coefC, C));

       //Ajout de Q
       this->vVertices().push_back(Vertex(Q, 1, 1));
   }
}
