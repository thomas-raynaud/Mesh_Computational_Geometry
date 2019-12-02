#include "meshruppert.h"

MeshRuppert::MeshRuppert(){
    Mesh2D();// Donne deja les point 0,1,..,6 cf Mesh2D

    insertion(Point(3, 4, 0)); //7
    insertion(Point(-2, 4, 0)); //8
    insertion(Point(1, 8, 0)); //9
    insertion(Point(-1, 9, 0)); //10
    insertion(Point(-7, 12, 0)); //11
    insertion(Point(-12, 7, 0)); //12
    insertion(Point(-3, 3, 0)); //13
    insertion(Point(-3, 2, 0)); //14
    insertion(Point(-5, 1, 0)); //15
    insertion(Point(-3, -1, 0)); //16

    _constraint = {{7,6}, {6,8}, {8,9}, {9,10}, {10,11}, {11,12}, {12,13}, {13,14}, {14,15}, {15,16}, {16,4}, {4,5}, {5,7}};


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
void MeshRuppert::splitEdge(int edge_index){
        std::array<int, 2> edge = _constraint[edge_index];
        int idx = vertexTab.size();
        Point a = vertexTab[edge[0]].point();
        Point b = vertexTab[edge[1]].point();
        insertion(Point((a.x()+b.x())*0.5, (a.y()+b.y())*0.5, 0));
        _constraint.remove(edge_index);
        _constraint.push_back({edge[0], idx});
        _constraint.push_back({idx, edge[1]});
}
void MeshRuppert::priority(){
   while(edgeNotInDel().size()){
        splitEdge(edgeNotInDel()[0]);
    }
}

double MeshRuppert::cos(Point a, Point b, Point c){
    Point ba = difference(a, b);
    Point bc = difference(c, b);
    if(dotProduct(ba, bc) > ba.norm()*bc.norm()){
        return -1;
    }else{
        return dotProduct(ba, bc)/(ba.norm()*bc.norm());
    }
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
            if(angl[0] > angl[1] && angl[0] > angl[2]){
                angl_tmp = a[0];
            }else if(angl[1] > angl[0] && angl[1] > angl[2]){
                angl_tmp = angl[1];
            }else{
                angl_tmp = angl[2];
            }
            if(angl_tmp > alpha){
                if(angl_tmp > angl_max){
                    angl_max = angl_tmp;
                    answ = itf->idx();
                }
            }
        }
    }
    if(abs(angl_max - 1) < 0.00001){
        return -1 ;

    }else{
        return answ;
    }
}

void MeshRuppert::raffinement(double alpha){
    priority();
    int fidx = findWorstTriangle(alpha);
    while(fidx != -1){
        Face face = faceTab[fidx];
        //Caclul du centre du certcle circonscrit
        Point A, B, C;
        A = vertexTab[face.vertices()[0]].point();
        B = vertexTab[face.vertices()[1]].point();
        C = vertexTab[face.vertices()[2]].point();

        //insertion du centre de voronoi dans le maillage test
        Point Q = computeCenter(A, B, C);
        MeshRuppert mesh_test = *this;
        mesh_test.insertion(Q);
        QVector<int> edges = mesh_test.edgeNotInDel();
        if(edges.size() == 0){
            insertion(Q);
        }else{

            //Quand une arrête est accroché je les split et je cherche les pire triangle crée.
            //Les triangles crées sont ceux incidents au sommet crée
            double angl_max = -1;
            int worst_face = -1;
            int offset = 0;
            for( int edge_index = 0; edge_index < edges.size(); edge_index++){
                splitEdge(edges[edge_index] - offset);
                offset++;
                int vidx = this->vertexTab.size() - 1;
                Circulator_on_faces cf;
                cf = incident_faces(vertexTab[vidx]);
                do{
                    if(isFaceVisible(cf->idx())){
                        int a_index = cf->vertices()[0];
                        int b_index = cf->vertices()[1];
                        int c_index = cf->vertices()[2];
                        Point a = vertexTab[a_index].point();
                        Point b = vertexTab[b_index].point();
                        Point c = vertexTab[c_index].point();

                        //Calcul des angles
                        std::array<double, 3> angl = {cos(b, a, c), cos(a, b, c), cos(b, c, a)};
                        double angl_tmp;
                        if(angl[0] > angl[1] && angl[0] > angl[2]){
                            angl_tmp = a[0];
                        }else if(angl[1] > angl[0] && angl[1] > angl[2]){
                            angl_tmp = angl[1];
                        }else{
                            angl_tmp = angl[2];
                        }
                        if(angl_tmp > alpha && angl_tmp < 1){
                            if(angl_tmp > angl_max){
                                angl_max = angl_tmp;
                                worst_face = cf->idx();
                            }
                        }
                    }
                        ++cf;
                }while(cf != incident_faces(vertexTab[vidx]));
            }
            //Ajout du pire centre de cercle circonscrit
            if(worst_face != -1){
                Point A, B, C;
                A = vertexTab[faceTab[worst_face].vertices()[0]].point();
                B = vertexTab[faceTab[worst_face].vertices()[1]].point();
                C = vertexTab[faceTab[worst_face].vertices()[2]].point();

                Q = computeCenter(A, B, C);
                insertion(Q);
            }
        }//FIn cas d'accroche

        fidx = findWorstTriangle(alpha);
    }
}
