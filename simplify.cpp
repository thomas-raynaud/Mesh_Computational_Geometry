#include "mesh.h"

std::array<int,2> Mesh::edgeCollapse(int vertexEd, int faceEd){
    //Recupération de v1
    int v1 = -1;
    int v2, loc_fc1_v1, loc_fc1_v2, loc_fc1_vertexEd;
    //Récupération de v2
    for(int i = 0; i < 3; i++){
        if(faceTab[faceEd].vertices()[i] != vertexEd){
            if(v1 == -1){
                v1 = faceTab[faceEd].vertices()[i];
                loc_fc1_v1 = i;
            }else{
                v2 = faceTab[faceEd].vertices()[i];
                loc_fc1_v2 = i;
            }
        }else{
            loc_fc1_vertexEd = i;
        }
    }

    //Récuperation de fc1
    int fc1 = faceEd;
    //Récupération de fc2
    int fc2 = faceTab[fc1].adjacentFaces()[loc_fc1_v1];

    //Recherche de l'indice local de v1 dans fc2
    int loc_fc2_v1, loc_fc2_v2;
    for (int i = 0; i < 3; i++){
        if(faceTab[fc2].vertices()[i] == v1){
            loc_fc2_v1 = i;
        }
        if(faceTab[fc2].vertices()[i] == v2){
            loc_fc2_v2 = i;
        }
    }

    //boucle circulateur faces sur v1 en partant de fc1
    Circulator_on_faces cf;
    cf = incident_faces(vertexTab[v1], fc1);
    cf++;

    //--Ratachement de la première face
    //----Recherche de la faca à lui ratacher, c'est l'opposé à fc1 du vertex v1
    int opositFace = faceTab[fc1].adjacentFaces()[loc_fc1_v1];

    //----Recherche de l'indice local du vertex opposer à fc1 dans la face à ratacher
    int localIndex, localIndex2ratach, localIndex1stFace;
    for(int i =0; i < 3; i++){
        localIndex = faceTab[opositFace].vertices()[i];
        if((localIndex != vertexEd) && (localIndex != v2)){
           localIndex2ratach = i;
           break;
        }
    }

    //----Recherche de l'indice local du vertex oposé à fc1 dans la première face
    for(int i =0; i < 3; i++){
        localIndex = faceTab[cf->idx()].vertices()[i];
        if((localIndex != vertexEd) && (localIndex != v1)){
            localIndex1stFace = i;
           break;
        }
    }

    //----Ratachement de la première face à la face a ratacher
    faceTab[opositFace].setAdjacentFace(cf->idx(), localIndex2ratach);

    //----Ratachement de la face à ratacher à la première face
    faceTab[cf->idx()].setAdjacentFace(opositFace, localIndex1stFace);

    //Sauvegarde de l'indice de la première face pour la maj des faces incidentes aux vertex à la fin
    int fstFace = cf->idx();

    //--Changement de point dans les faces
    do{
        //----Recherche de l'indice local de v1 dans cf
        int loc_cf_v1;
        std::array<int, 3> newVert;
        for(int i = 0; i < 3; i++){
            if(cf->vertices()[i] == v1){
                newVert[i] = v1;
            }else{
                newVert[i] = cf->vertices()[i];
            }
        }

        //----Changement
        cf->setVertices(newVert);
    }while(cf->idx() != fc2);

    //--Ratachement de la dernière face
    --cf;
    //----Recherche du vertex oposé à vertexEd, il joue un rôle similaire
    int vertexEdOp, loc_fc2_vertexEdOp;
    for(int i = 0; i < 3; i++){
       localIndex = faceTab[fc2].vertices()[i];
       if((localIndex != v1) && (localIndex != v2)){
           vertexEdOp = localIndex;
           loc_fc2_vertexEdOp = i;
       }
    }
    //----Recherche de la face à lui ratacher, c'est l'opposé de fc2 relativement à v1
    opositFace = faceTab[fc2].adjacentFaces()[loc_fc2_v1];

    //----Recherche de l'indice local du vertex oposé à fc2 dans la dernière face
    for(int i = 0; i < 3; i++){
        localIndex = faceTab[opositFace].vertices()[i];
        if((localIndex != vertexEdOp) && (localIndex != v2)){
            localIndex2ratach = i;
            break;
        }
    }

    //----Recherche de l'indice local du vertex oposé à fc2 dans la face a ratacher
    for(int i = 0; i < 3; i++){
        localIndex = faceTab[cf->idx()].vertices()[i];
        if((localIndex != vertexEdOp)&& (localIndex != v1)){
            localIndex1stFace = i;
        }
    }

    //----Ratachement de ladernière première faca à la face à ratacher
    faceTab[opositFace].setAdjacentFace(cf->idx(), localIndex2ratach);

    //----Ratachement de la face a ratacher à la dernière face
    faceTab[cf->idx()].setAdjacentFace(opositFace, localIndex1stFace);

    //Saubegarde de la dernière face pour la maj des faces incidentes aux vertex
    int lstFace = cf->idx();

    //Correction des face incidentes au vertex des faces supprimées
    vertexTab[faceTab[fc1].vertices()[loc_fc1_vertexEd]].setFace(fstFace);
    vertexTab[faceTab[fc1].vertices()[loc_fc1_v2]].setFace(fstFace);
    vertexTab[faceTab[fc2].vertices()[loc_fc2_vertexEdOp]].setFace(lstFace);

    return {fc1, fc2};
};
