#include<vector>
#include "mesh.h"
#include "point.h"


std::array<int,3> Mesh::edgeCollapse(int vertexEd, int faceEd){
    /*---------------------------------------------------------------------------------
     * ---------Préparation du terrin, recuperation des indices------------------------
     * --------------------------------------------------------------------------------*/
    //Recupération de v1, son idice local dans fc1
    int v1 = -1;
    int v2, loc_fc1_v1, loc_fc1_v2, loc_fc1_vertexEd;
    //Alternative
    for(int i = 0; i < 3; i++){
        if(faceTab[faceEd].vertices()[i] == vertexEd){
            loc_fc1_vertexEd = i;
            break;
        }
    }
    loc_fc1_v1 = (loc_fc1_vertexEd +1)%3;
    v1 = faceTab[faceEd].vertices()[loc_fc1_v1];
    loc_fc1_v2 = (loc_fc1_v1 + 1)%3;
    v2 = faceTab[faceEd].vertices()[loc_fc1_v2];

    //Récuperation de fc1
    int fc1 = faceEd;
    //Récupération de fc2
    int fc2 = faceTab[fc1].adjacentFaces()[loc_fc1_vertexEd];

    //Recherche de l'indice local de v1 dans fc2 ainsi que du vertex opose à vertexEd
    int loc_fc2_v1, loc_fc2_v2, vertexEdOp, loc_fc2_vertexEdOp;
    for (int i = 0; i < 3; i++){
        if(faceTab[fc2].vertices()[i] == v1){
            loc_fc2_v1 = i;
        }else if(faceTab[fc2].vertices()[i] == v2){
            loc_fc2_v2 = i;
        }else{
            loc_fc2_vertexEdOp = i;
            vertexEdOp = faceTab[fc2].vertices()[i];
        }
    }
    /*------------------------------------------------------------------------------------
     * -----Changement sur les faces, circulateur sur les faces de v1---------------------
     * -----------------------------------------------------------------------------------*/

    //boucle circulateur faces sur v1 en partant de fc1
    Circulator_on_faces cf;
    cf = incident_faces(vertexTab[v1], fc1);
    cf++;

    //Sauvegarde de l'indice de la première face pour la maj des faces incidentes aux vertex à la fin
    int fstFace = cf->idx();
    int lstFace;

    //--Changement de vertex dans les faces
    do{
        //----Recherche de l'indice local de v1 dans cf

        int loc_cf_v1;
        std::array<int, 3> newVert;
        for(int i = 0; i < 3; i++){
            if(cf->vertices()[i] == v1){
                //On remplace l'indice de v1 par celui de v2 dans la face cf
                newVert[i] = v2;
            }else{
                newVert[i] = cf->vertices()[i];
            }
        }

        //----Changement
        cf->setVertices(newVert);
        lstFace = cf->idx();
        cf++;
        /*
        std::cout<<cf->idx();
        std::cout<<" ";
        std::cout<<fc2<<std::endl;
        */
    }while(cf->idx() != fc2);

    //--Ratachement de la première face
    //----Recherche de la faca à lui ratacher, c'est l'opposé à fc1 du vertex v1
    int opositFace = faceTab[fc1].adjacentFaces()[loc_fc1_v1];

    //----Recherche de l'indice local du vertex opposé à fc1 dans la face à ratacher
    int index, localIndex2ratach, localIndex1stFace;
    for(int i =0; i < 3; i++){
        index = faceTab[opositFace].vertices()[i];
        if((index != vertexEd) && (index != v2)){
           localIndex2ratach = i;
           break;
        }
    }

    //----Recherche de l'indice local du vertex oposé à fc1 dans la première face
    for(int i =0; i < 3; i++){
        index = faceTab[fstFace].vertices()[i];
        if((index != vertexEd) && (index != v2)){
            localIndex1stFace = i;
           break;
        }
    }

    //----Ratachement de la première face à la face a ratacher
    faceTab[opositFace].setAdjacentFace(fstFace, localIndex2ratach);

    //----Ratachement de la face à ratacher à la première face
    faceTab[fstFace].setAdjacentFace(opositFace, localIndex1stFace);

    //DERNIERE FACE------------------------------------------------------------------------
    //--Ratachement de la dernière face

    //----Recherche de la face à lui ratacher, c'est l'opposé de fc2 relativement à v1
    opositFace = faceTab[fc2].adjacentFaces()[loc_fc2_v1];

    //----Recherche de l'indice local du vertex oposé à fc2 dans la dernière face
    for(int i = 0; i < 3; i++){
        index = faceTab[opositFace].vertices()[i];
        if((index != vertexEdOp) && (index != v2)){
            localIndex2ratach = i;
            break;
        }
    }

    //----Recherche de l'indice local du vertex oposé à fc2 dans la face a ratacher
    for(int i = 0; i < 3; i++){
        index = faceTab[lstFace].vertices()[i];
        if((index != vertexEdOp)&& (index != v2)){
            localIndex1stFace = i;
        }
    }

    //----Ratachement de ladernière première faca à la face à ratacher
    faceTab[opositFace].setAdjacentFace(lstFace, localIndex2ratach);

    //----Ratachement de la face a ratacher à la dernière face
    faceTab[lstFace].setAdjacentFace(opositFace, localIndex1stFace);


    //Correction des face incidentes au vertex des faces supprimées
    vertexTab[vertexEd].setFace(fstFace);
    vertexTab[v2].setFace(fstFace);
    vertexTab[vertexEdOp].setFace(lstFace);

    return {v1, fc1, fc2};
};

void Mesh::simplify(int n){
    //Le paramètre est le nombre d'arrete à supprimer

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
