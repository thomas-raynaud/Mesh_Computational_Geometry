#include "meshruppert.h"

MeshRuppert::MeshRuppert(){
    Mesh2D();// Donne deja les point 0,1,..,6 cf Mesh2D

    insertion(Point(-5, -4, 0)); //7
    insertion(Point(-8, -2, 0)); //8
    insertion(Point(-6, 6, 0)); //9
    insertion(Point(-2, 6, 0)); //10
    insertion(Point(1, 10, 0)); //11
    insertion(Point(5, 4, 0)); //12
    insertion(Point(1, 3, 0)); //13
    insertion(Point(-6, -8, 0)); //14

    //_constraint = {{4,5}, {7,8}, {8,9}, {9,10}, {10,11}, {12,13}, {13,14}, {14,12}};
   // _constraint = {  {11,12}, {12,13}, {13,5}, {5,6}, {6,4}};
    _constraint = {{4,7}, {7,14}, {14,8}, {8,9}, {9,10}, {10,11}, {11,12}, {12,13}, {13,5}, {5,6}, {6,4}};
 //   _constraint = {{13,5}, {5,6}, {6,4}};

    priority();

   //raffinement(0.9);
}

bool MeshRuppert::isConstraint(int a, int b){
    bool is = false;
    for (int i = 0; i < constraint().size(); i++){
        std::array<int, 2> edge = constraint()[i];
        is = is || ((edge[0] == a && edge[1]==b) || (edge[0]==b && edge[1]==a));
    }
    return is;
}

QVector<int> MeshRuppert::edgeNotInDel(){
    QVector<int> answ;
    for(int i = 0; i < constraint().size(); i++){
        std::array<int, 2> edge = constraint()[i];
        Circulator_on_vertices cv, cv_begin;
        cv_begin = this->neighbour_vertices(vertexTab[edge[0]]);
        cv = cv_begin;
        bool init = false;
        do{
            init = init || (edge[1]==cv->idx());
            ++cv;
        }while(cv != cv_begin);
        if (!init){
            answ.push_back(i);
        }
    }
    return answ;
}
void MeshRuppert::splitEdge(std::array<int, 2> edge){
        int idx = vertexTab.size();
        Point a = vertexTab[edge[0]].point();
        Point b = vertexTab[edge[1]].point();
        insertion(Point((a.x()+b.x())*0.5, (a.y()+b.y())*0.5, 0));
        _constraint.remove(edgeNotInDel()[0]);
        _constraint.push_back({edge[0], idx});
        _constraint.push_back({idx, edge[1]});
}
void MeshRuppert::priority(){
   while(edgeNotInDel().size()){
        std::array<int,2> edge = constraint()[edgeNotInDel()[0]];
        splitEdge(edge);
    }
}

double MeshRuppert::cos(Point a, Point b, Point c){
    Point ba = difference(a, b);
    Point bc = difference(c, b);
    return dotProduct(ba, bc)/(ba.norm()*bc.norm());
}

int MeshRuppert::findWorstTriangle(int alpha){
    int answ = -1;
    double angl_max = -1;
    Iterator_on_faces itf;
    for(itf = this->faces_begin(); itf != this->faces_end(); ++itf){
        if(isFaceVisible(itf->idx())){
            int a_index = itf->vertices()[0];
            int b_index = itf->vertices()[1];
            int c_index = itf->vertices()[2];
            Point a = vertexTab[a_index].point();
            Point b = vertexTab[b_index].point();
            Point c = vertexTab[c_index].point();

            //Calcul des angles
            std::array<double, 3> angl = {cos(b, a, c), cos(a, b, c), cos(b, c, a)};
            double angl_tmp;
            if(angl[0]>angl[1]){
                if(angl[0]>angl[2]){
                    angl_tmp = angl[0];
                }else{
                    angl_tmp = angl[2];
                }
            }else{
                if(a[1]>a[2]){
                    angl_tmp = angl[1];
                }else{
                    angl_tmp= angl[2];
                }
            }
            if(angl_tmp > alpha){
                if(angl_tmp > angl_max){
                    angl_max = angl_tmp;
                    answ = itf->idx();
                }
            }
        }
    }
    return answ;
}

void MeshRuppert::raffinement(double alpha){
    int fidx = findWorstTriangle(alpha);
    //while(fidx != -1){
        Face face = faceTab[fidx];
        //Caclul du centre du certcle circonscrit
        Point A, B, C;
        A = vertexTab[face.vertices()[0]].point();
        B = vertexTab[face.vertices()[1]].point();
        C = vertexTab[face.vertices()[2]].point();

        //insertion du centre de voronoi dans le maillage test
        Point Q = computeCenter(A, B, C);
        insertion(Q);

        //Recuperation des arrêtes accroché
        fidx = findWorstTriangle(alpha);
    //}
}
