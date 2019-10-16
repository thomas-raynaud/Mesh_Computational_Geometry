#include "mesh.h"

void Mesh2D::buildVoronoi(){
    int i =4;
    _vVertices = QVector<Vertex>();
   for (int faceIndex = 0; faceIndex < this->faceTab.size(); faceIndex++){
       Vertex AV, BV, CV;
       Point A, B, C;
       AV = this->vertexTab[this->faceTab[faceIndex].vertices()[0]];
       BV = this->vertexTab[this->faceTab[faceIndex].vertices()[1]];
       CV = this->vertexTab[this->faceTab[faceIndex].vertices()[2]];
       //if(!(AV.isFictive() || BV.isFictive() || CV.isFictive() )){
           A = AV.point();
           B = BV.point();
           C = CV.point();
           //Calcul de Q
           double tanA, tanB, tanC, coefA, coefB, coefC;
           tanA = tan(C, A, B);
           tanB = tan(A, B, C);
           tanC = tan(B, C, A);

           coefA = 0.5*(tanC + tanB);
           coefB = 0.5*(tanC + tanA);
           coefC = 0.5*(tanA + tanB);
           double sum = 1 / (coefA + coefB + coefC);
           //double sum = 1;
           coefA = coefA * sum;
           coefB = coefB * sum;

           Point Q;
           Q = somme(somme(scalarProduct(coefA, A), scalarProduct(coefB, B)), scalarProduct(coefC, C));

           //std::cout<<Q.x()<<" "<<Q.y()<<" "<<Q.z()<<std::endl;

           //Ajout de Q
           _vVertices.push_back(Vertex(Q, 0, i));
           i++;
      // }
   }
}
