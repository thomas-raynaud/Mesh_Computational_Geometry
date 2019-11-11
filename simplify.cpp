#include<vector>
#include "mesh.h"
#include "point.h"

// VE : sommet opposé à l'arête à supprimer dans la face FE
// FE : face qui a une de ses arêtes à supprimer, et a pour sommet FE.
std::array<int, 3> Mesh::edgeCollapse(int VE, int FE) {

    int V1, V2; // Sommets de l'arête à supprimer. V1, V2 et VE forment FE.
    int FE_Opp; // Face opposée à VE depuis la face FE
    int FA1, FA2; // Faces adjacentes à FE. -> FA1 et FA2 != FE_Opp
    int FA3, FA4; // Faces adjacentes à FE_Opp. -> FA3 et FA4 != FE

    Circulator_on_faces cof; // Circulateur sur les faces de V1 et V2

    // Trouver V1, V2, FA1, FA2 et FE_Opp
    for (int i = 0; i < 3; ++i) {
        if (faceTab[FE].vertices()[i] == VE) {
            FE_Opp = faceTab[FE].adjacentFaces()[i];
            V1 = faceTab[FE].vertices()[(i + 1) % 3];
            V2 = faceTab[FE].vertices()[(i + 2) % 3];
            FA1 = faceTab[FE].adjacentFaces()[(i + 1) % 3];
            FA2 = faceTab[FE].adjacentFaces()[(i + 2) % 3];
            break;
        }
    }
    // Trouver FA3 et FA4
    for (int i = 0; i < 3; ++i) {
        if (faceTab[FE].vertices()[i] == V1) {
            FA3 = faceTab[FE].adjacentFaces()[i];
            FA4 = faceTab[FE].adjacentFaces()[(i - 1) % 3];
            break;
        }
    }

    // Remplacer V1 par V2 pour les faces adjacentes à V1
    cof = incident_faces(vertexTab[V1], FE);
    ++cof;
    while (cof->idx() != FE || cof->idx() != FE_Opp) {
        for (int i = 0; i < 3; ++i) {
            if(faceTab[cof->idx()].vertices()[i] == V1){
                faceTab[cof->idx()].setVertice(V2, i);
            }
            // Changer la face pointée par chaque sommet
            vertexTab[faceTab[cof->idx()].vertices()[i]].setFace(cof->idx());
        }
        ++cof;
    }

    // Connecter FA1 <-> FA2, FA3 <-> FA4
    for (int i = 0; i < 3; ++i) {
        // FA1 <-> FA2
        if (faceTab[FA1].adjacentFaces()[i] == FE)
            faceTab[FA1].setAdjacentFace(FA2, i);
        if (faceTab[FA2].adjacentFaces()[i] == FE)
            faceTab[FA2].setAdjacentFace(FA1, i);
        //FA3 <-> FA4
        if (faceTab[FA3].adjacentFaces()[i] == FE_Opp)
            faceTab[FA3].setAdjacentFace(FA4, i);
        if (faceTab[FA4].adjacentFaces()[i] == FE_Opp)
            faceTab[FA4].setAdjacentFace(FA3, i);
    }

    // Changer les coordonnées de V2 : au milieu de l'arête à supprimer
    // (V1 + V2) * 0.5
    vertexTab[V2].setPoint((vertexTab[V1].point() + vertexTab[V2].point()) * 0.5);

    // On retourne les éléments à supprimer du Mesh : V1, FE et FE_Opp
    return {V1, FE, FE_Opp};
}


// n : nombre d'arêtes à supprimer
void Mesh::simplify (int n) {

    //Ranger les arretes
    std::vector<int> face, loc_face;
    std::vector<double> size;
    
    Iterator_on_faces itf;

    for(itf = this->faces_begin(); itf != this->faces_end(); itf++){
        for(int localVertexIndex = 0; localVertexIndex < 3; localVertexIndex++){
            //On a un arrete, il faut savoir si on l'à déjà mesurée
            //Ou plus précisement, si on a déjà mesure l'arrête conjuguée
            //Cherche l'arrete conjuguée
            int opositFace = itf->adjacentFaces()[localVertexIndex];
            std::array<int,2> ab;
            ab = {itf->vertices()[(localVertexIndex+1)%3], itf->vertices()[(localVertexIndex+2)%3]};
            int loc_opositeFace_notab;
            for(int i = 0; i < 3; i++){
                if(faceTab[opositFace].adjacentFaces()[i] == itf->idx()){
                    loc_opositeFace_notab = i;
                    break;
                }
            }

            //Chercher si l'arrete conjugué à déjà été entrée
            bool init = false;
            for(int edgeIndex = 0; edgeIndex < face.size(); edgeIndex++){
                init = init || ((face[edgeIndex] == opositFace) && (loc_face[edgeIndex] == loc_opositeFace_notab));
            }
            if(!init){
                //Si l'arrete n'a pas été déjà entrée
                //Chercher si l'arete est sur le bord, ie si un de vertex servant a la décrir est ficitf
                bool isVisible;
                isVisible = isFaceVisible(itf->idx())||isFaceVisible(opositFace);

                if(isVisible){
                    //Si l'arete n'est pas fictive et qu'on a pas déjà ajouté sa conjuguée, on peut l'ajouter
                    face.push_back(itf->idx());
                    loc_face.push_back(localVertexIndex);
                    size.push_back(difference(vertexTab[ab[0]].point(), vertexTab[ab[1]].point()).norm());
                }
            }

        }
    }

    //Tri Naif
    std::vector<double> sizeC;
    std::cout<<"size : ";
    for(int i =0; i < size.size(); i++){
        std::cout<<size[i]<<" ";
        sizeC.push_back(size[i]);
    }
    std::cout<<std::endl;
    std::vector<int> indices;
    for(int i = 0; i < size.size(); i++){
        indices.push_back(i);
    }
    for(int i = 0; i < size.size(); i++){
        int minInd = i;
        for(int j = i; j < size.size(); j++){
            if(size[j] < size[minInd]){
                minInd = j;
            }
        }
        double tmp = size[i];
        size[i] = size[minInd];
        size[minInd] = tmp;
        tmp = indices[i];
        indices[i] = indices[minInd];
        indices[minInd] = tmp;
    }

    std::cout<<"size after : ";
    for(int i =0; i < size.size(); i++){
        std::cout<<size[i]<<" ";
    }
    std::cout<<std::endl;


    //application des permutations
    std::vector<int> faceSorted, loc_faceSorted;
    std::vector<double> sizeSorted;
    for(int i = 0; i < indices.size(); i++){
        faceSorted.push_back(face[indices[i]]);
        loc_faceSorted.push_back((loc_face[indices[i]]));
        sizeSorted.push_back(sizeC[indices[i]]);
    }

    std::cout<<"sizeSorted : ";
    for(int i =0; i < sizeSorted.size(); i++){
        std::cout<<sizeSorted[i]<<" ";
    }
    std::cout<<std::endl;


    //collapse du nombre voulu d'arrêtes
    if(n > face.size()){
        n = face.size();
    }
    std::array<int,3> tmp;
    for(int i = 0; i < n; i++){
        tmp = edgeCollapse(faceTab[faceSorted[i]].vertices()[loc_faceSorted[i]], faceSorted[i]);
        vertexPop(tmp[0]);
        facePop(tmp[1]);
        facePop(tmp[2]);
    }
}
