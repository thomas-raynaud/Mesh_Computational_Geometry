#include "mesh.h"

void Mesh2D::buildVoronoi(){
    int i =4;
    _vVertices = QVector<Vertex>();
   for (QVector<Face>::iterator face_it = faceTab.begin(); face_it != faceTab.end(); ++face_it){
       Vertex AV, BV, CV;
       Point A, B, C;
       AV = vertexTab[face_it->vertices()[0]];
       BV = vertexTab[face_it->vertices()[1]];
       CV = vertexTab[face_it->vertices()[2]];
       //if(!(AV.isFictive() || BV.isFictive() || CV.isFictive() )){
           A = AV.point();
           B = BV.point();
           C = CV.point();
           //Calcul de Q
           double tanA, tanB, tanC, coefA, coefB, coefC;
           tanA = tangente(C, A, B);
           tanB = tangente(A, B, C);
           tanC = tangente(B, C, A);

           coefA = (tanC + tanB);
           coefB = (tanC + tanA);
           coefC = (tanA + tanB);
           double sum = 1.0 / (coefA + coefB + coefC);
           //double sum = 1;
           coefA = coefA * sum;
           coefB = coefB * sum;
           coefC = coefC * sum;

           Point Q = coefA * A + coefB * B + coefC * C;
           //Q = somme(somme(scalarProduct(coefA, A), scalarProduct(coefB, B)), scalarProduct(coefC, C));

           //std::cout<<Q.x()<<" "<<Q.y()<<" "<<Q.z()<<std::endl;

           //Ajout de Q
           _vVertices.push_back(Vertex(Q, 0, i));
           i++;
      // }
   }
}
