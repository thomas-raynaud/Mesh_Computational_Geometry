#include "meshruppert.h"

MeshRuppert::MeshRuppert(){
    Mesh2D();// Donne deja les point 0,1,..,6 cf Mesh2D

    insertion(Point(-2, 1, 0)); //7
    insertion(Point(0, 4, 0)); //8
    insertion(Point(0, 3, 0)); //9
    insertion(Point(1, 3, 0)); //10
    insertion(Point(2, 4, 0)); //11
    insertion(Point(4, 1, 0)); //12
    insertion(Point(5, 0, 0)); //13
    insertion(Point(4, 0, 0)); //14

    //_constraint = {{4,5}, {7,8}, {8,9}, {9,10}, {10,11}, {12,13}, {13,14}, {14,12}};
    _constraint = {{14,9}, {9,11}, {11,10}, {8,4}};


}

bool MeshRuppert::isConstraint(int a, int b){
    bool is = false;
    for (int i = 0; i < constraint().size(); i++){
        std::array<int, 2> edge = constraint()[i];
        is = is || ((edge[0] == a && edge[1]==b) || (edge[0]==b && edge[1]==a));
    }
    return is;
}
