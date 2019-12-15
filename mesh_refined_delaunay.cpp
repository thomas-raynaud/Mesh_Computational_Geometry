#include "mesh_refined_delaunay.h"

Mesh_Constrained::Mesh_Constrained() {
    Mesh2D();
    insertion(Point(-2, 1, 0));
    insertion(Point(0, 4, 0));
    insertion(Point(0, 3, 0));
    insertion(Point(1, 3, 0));
    insertion(Point(2, 4, 0));
    insertion(Point(4, 1, 0));
    insertion(Point(4, 0, 0));
    insertion(Point(5, 0, 0));




    // Ajouter les arêtes contraintes
    faceTab.push_back(Face());

    //constraints.push_back();
}
