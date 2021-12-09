#include<vector>
#include "meshes/mesh.h"
#include "utils/point.h"

// VE : sommet opposé à l'arête à supprimer dans la face FE
// FE : face qui a une de ses arêtes à supprimer
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

    // Vérifier si FA1 / FA2 et FA3 / FA4 ne sont pas déjà connectés
    for (int i = 0; i < 3; ++i) {
        if (faceTab[FA1].adjacentFaces()[i] == FA2 || faceTab[FA3].adjacentFaces()[i] == FA4) {
            return {-1, -1, -1};
        }
    }

    // Remplacer V1 par V2 pour les faces adjacentes à V1
    cof = incident_faces(m_vertices[V1], FE);
    ++cof;
    while (cof->idx() != FE_Opp) {
        for (int i = 0; i < 3; ++i) {
            if(faceTab[cof->idx()].vertices()[i] == V1){
                faceTab[cof->idx()].setVertice(V2, i);
            }
            // Changer la face pointée par chaque sommet
            m_vertices[faceTab[cof->idx()].vertices()[i]].setFace(cof->idx());
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
    m_vertices[V2].setPoint((m_vertices[V1].point() + m_vertices[V2].point()) * 0.5);

    // On retourne les éléments à supprimer du Mesh : V1, FE et FE_Opp
    return {V1, FE, FE_Opp};
}

double Mesh::getEdgeLength(Edge e) {
    Point a = m_vertices[e.v1].point();
    Point b = m_vertices[e.v2].point();
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
    if (m_vertices.size() <= n) return;

    std::vector<Edge> edges;
    std::vector<int> deletedFaces;
    std::vector<int> deletedVertices;
    std::array<int, 3> deletedElements;
    Edge e;
    int nbVertices = m_vertices.size(), f;
    bool edgeNew, faceHidden;
    compareEdges.mesh = this;

    // Récupérer les arêtes du mesh
    //edges = getEdges();

    for(Iterator_on_faces itf = faces_begin(); itf != faces_end(); ++itf) {
        f = itf->idx();

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
    while (n < nbVertices && edges.size() > 0) {
    //while (m_vertices.size() <= nbVertices && edges.size() > 0) {
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
                        || edges[i].ve == edges[i].v1 || edges[i].v1 == edges[i].v2 || edges[i].v2 == edges[i].ve) { // les sommets de l'arête ont fusionné
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
