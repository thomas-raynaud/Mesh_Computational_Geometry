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
        if (faceTab[FE_Opp].vertices()[i] == V1) {
            FA3 = faceTab[FE_Opp].adjacentFaces()[i];
            FA4 = faceTab[FE_Opp].adjacentFaces()[(i + 2) % 3];
            break;
        }
    }

    // Les faces incidentes à l'arête et les faces adjacentes doivent être visibles
    if (!isFaceVisible(FA1) || !isFaceVisible(FA2) || !isFaceVisible(FA3) || !isFaceVisible(FA4)
            || !isFaceVisible(FE) || !isFaceVisible(FE_Opp)) {
        return {-1, -1, -1};
    }

    // Vérifier si FA1 / FA2 et FA3 / FA4 ne sont pas déjà connectés
    for (int i = 0; i < 3; ++i) {
        if (faceTab[FA1].adjacentFaces()[i] == FA2 || faceTab[FA3].adjacentFaces()[i] == FA4) {
            return {-1, -1, -1};
        }
    }

    // Remplacer V1 par V2 pour les faces adjacentes à V1
    cof = incident_faces(vertexTab[V1], FE);
    ++cof;
    while (cof->idx() != FE_Opp) {
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

double Mesh::getEdgeLength(Edge e) {
    Point a = vertexTab[e.v1].point();
    Point b = vertexTab[e.v2].point();
    return difference(a, b).norm();
}

struct comparatorEdges {
  bool operator() (Edge e1, Edge e2) {
      return mesh->getEdgeLength(e1) < mesh->getEdgeLength(e2);
  }
  Mesh *mesh;
} compareEdges;


// n : nombres de sommets après simplification
void Mesh::simplify (int n) {
    // Pas de simplification si le nombre de sommets à obtenir est supérieur au
    // nombre de sommets actuels.
    if (vertexTab.size() <= n) return;

    std::vector<Edge> edges;
    std::vector<int> deletedFaces;
    std::vector<int> deletedVertices;
    std::array<int, 3> deletedElements;
    Edge e;
    int nbVertices = vertexTab.size(), f;
    bool edgeNew, faceHidden;
    compareEdges.mesh = this;

    // Récupérer les arêtes du mesh
    //edges = getEdges();

    for(Iterator_on_faces itf = faces_begin(); itf != faces_end(); ++itf) {
        f = itf->idx();
        if (!isFaceVisible(f)) continue;

        // Ne pas prendre les arêtes ayant des faces incidentes en bordure
        faceHidden = false;
        for (uint i = 0; i < 3; ++i) {
            if (!isFaceVisible(faceTab[f].adjacentFaces()[i])) {
                faceHidden = true;
                break;
                for (uint j = 0; j < 3; ++j) {
                    if (!isFaceVisible(faceTab[faceTab[f].adjacentFaces()[i]].adjacentFaces()[j])) {
                        faceHidden = true;
                        break;
                    }
                }
            }
        }
        if (faceHidden) continue;

        // Créer l'arête
        for (uint i = 0; i < 3; ++i) {
            e.ve = faceTab[f].vertices()[i];
            e.fe = f;
            e.v1 = faceTab[f].vertices()[(i + 1) % 3];
            e.v2 = faceTab[f].vertices()[(i + 2) % 3];
            // Regarder si l'arête conjuguée est dans le tableau
            edgeNew = true;
            for (uint j = 0; j < edges.size(); ++j) {
                if (edges[j].v1 == e.v2 && edges[j].v2 == e.v1) {
                    edgeNew = false;
                    break;
                }
            }
            if (edgeNew) edges.push_back(e);
        }
    }

    // Tant que le maillage n'a pas n sommets ou qu'il n'y a plus d'arêtes
    //while (n < nbVertices && edges.size() > 0) {
    while (vertexTab.size() <= nbVertices && edges.size() > 0) {
        // Ordonner les arêtes par leur longueur (ordre croissant)
        std::sort(edges.begin(), edges.end(), compareEdges);
        // On supprime la plus petite arête
        deletedElements = edgeCollapse(edges[0].ve, edges[0].fe);

        if (deletedElements[0] != -1) { // Arête bien supprimée
            // réarranger les arêtes : changer les références des arêtes impactées

            for (uint i = 1; i < edges.size(); ++i) {
                int fe_opp;
                for (int j = 0; j < 3; ++j) {
                    if (faceTab[edges[i].fe].vertices()[j] == edges[i].ve) {
                        fe_opp = faceTab[edges[i].fe].adjacentFaces()[j];
                        break;
                    }
                }
                // Supprimer l'arête si ...
                if (edges[i].fe == deletedElements[1] || edges[i].fe == deletedElements[2] // une face incidente a été supprimée
                        || fe_opp == deletedElements[1] || fe_opp == deletedElements[2]
                        || edges[i].ve == deletedElements[0] || edges[i].v1 == deletedElements[0] || edges[i].v2 == deletedElements[0] // un sommet lié à l'arête a été supprimée
                        || edges[i].ve == edges[i].v1 || edges[i].v1 == edges[i].v2 || edges[i].v2 == edges[i].ve // les sommets de l'arête ont fusionné
                        || !isFaceVisible(edges[i].fe) || !isFaceVisible(fe_opp)) { // une des faces incidentes de l'arête est devenue invisible
                    edges.erase(edges.begin() + i);
                    --i;
                    continue;
                }
            }
            --nbVertices;
            deletedVertices.push_back(deletedElements[0]);
            deletedFaces.push_back(deletedElements[1]);
            deletedFaces.push_back(deletedElements[2]);
        }
        edges.erase(edges.begin());
    }

    // Nettoyer le maillage
    std::sort(deletedVertices.begin(), deletedVertices.end(), std::greater<int>());
    std::sort(deletedFaces.begin(), deletedFaces.end(), std::greater<int>());

    // Faces
    for (uint i = 0; i < deletedFaces.size(); ++i) {
        facePop(deletedFaces[i]);
    }
    // Sommets
    for (uint i = 0; i < deletedVertices.size(); ++i) {
        vertexPop(deletedVertices[i]);
    }
}

/*
std::array<int,3> Mesh::edgeCollapse(int vertexEd, int faceEd){
    //---------------------------------------------------------------------------------
    // ---------Préparation du terrin, recuperation des indices------------------------
    // --------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------------
    // -----Changement sur les faces, circulateur sur les faces de v1---------------------
    // -----------------------------------------------------------------------------------

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


// n : nombre d'arêtes à supprimer
void Mesh::simplify (int n) {

    // Ranger les arêtes
    std::vector<int> face, loc_face;
    std::vector<double> size;
    
    Iterator_on_faces itf;

    for(itf = this->faces_begin(); itf != this->faces_end(); ++itf){
        for(int localVertexIndex = 0; localVertexIndex < 3; ++localVertexIndex){
            // On a une arête, il faut savoir si on l'a mesurée
            // Ou plus précisement, si on a déjà mesuré l'arête conjuguée
            // Cherche l'arête conjuguée
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
*/
