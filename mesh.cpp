#include "mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>

void Face::addAdjacentFace(int faceIndex, int pos) {
    _adjacentFaces[pos] = faceIndex;
}

Mesh::Mesh() {}
Mesh::~Mesh() {}

std::vector<std::array<double, 3>> Mesh::getLaplacians() {
    if (laplacianTab.size() != 0) return laplacianTab;
    float A; // Aire
    Point a, b, c, d; // Vecteur du triangle pour les calculs
    std::vector<std::array<double, 3>> laplacians;
    // On utilise l'itérateur sur les vertices pour parcourir tous les vertex
    Iterator_on_vertices it_v;
    Circulator_on_faces cf, cfbegin;
    Circulator_on_vertices cv, cvbegin, cvtemp;
    double lap_x, lap_y, lap_z;
    double opp, adj, cot_alpha, cot_beta; // Calcul de trigo
    int i = -1;
    for (it_v = this->vertices_begin(); it_v !=this->vertices_past_the_end(); it_v++) { // on parcourt tous les sommets du mesh
        i++;
        cfbegin = this->incident_faces(*it_v);
        A = 0;
        cf = cfbegin;
        // On parcourt toutes les faces qui ont le sommet it_v
        do {
            // Calcul de l'aire
            a = vertexTab[cf->vertices()[0]].point();
            b = vertexTab[cf->vertices()[1]].point();
            c = vertexTab[cf->vertices()[2]].point();
            A = A + (1.f / 3.f) * ((1.f / 2.f) * (crossProduct(difference(b, a), difference(c, a))).norm());
            cf++;
        } while (cf != cfbegin);

        lap_x = lap_y = lap_z = 0;
        cvbegin = this->incident_vertices(*it_v);
        cv = cvbegin;
        a = it_v->point();
        // On parcourt les arêtes du sommet it_v
        do {
            cvtemp = cv;
            --cvtemp;
            b = cvtemp->point();
            cvtemp++;
            c = cvtemp->point();
            cvtemp++;
            d = cvtemp->point();
            // cot alpha
            opp = difference(a, c).norm();
            adj = difference(a, b).norm();
            cot_alpha = adj / opp;

            // cot beta
            adj = difference(a, d).norm();
            cot_beta = adj / opp;

            // sommes
            lap_x = lap_x + (cot_alpha + cot_beta) * (c.x() - a.x());
            lap_y = lap_y + (cot_alpha + cot_beta) * (c.y() - a.y());
            lap_z = lap_z + (cot_alpha + cot_beta) * (c.z() - a.z());

            cv++;
        } while (cv != cvbegin);
        // Calcul du Laplacien pour le sommet it_v;
        lap_x = (1.f / (2.f * A)) * lap_x;
        lap_y = (1.f / (2.f * A)) * lap_y;
        lap_z = (1.f / (2.f * A)) * lap_z;
        laplacians.push_back({lap_x, lap_y, lap_z});
    }
    laplacianTab = laplacians;
    return laplacians;
}

void Mesh::computeColors(int curveAxis) {
    double max;
    std::vector<std::array<double, 3>> laplacians = getLaplacians();
    if(curveAxis < 3) {
        max= 0.0;
        for (int i = 0; i < vertexTab.size(); ++i) {
            max = std::max(max, std::abs(laplacians[i][curveAxis]));
           }
        double hue = 0.0;
        for (int i = 0; i < vertexTab.size(); ++i) {
            hue = (std::abs(laplacians[i][curveAxis]) / max) * 270.0 + 90.0;
            vertexTab[i].setColor(hsv2rgb((int)hue, 1.0, 1.0));
        }
    } else if (curveAxis == 3) {

    } else {
        // ...
    }
}

void Mesh::connectAdjacentFaces() {
    std::unordered_map<std::string, int> edges; // map des arêtes du mesh
    std::array<int, 3> ind_vertices; // indice des sommets d'une face
    int first_v, scnd_v; // indices de sommets
    std::string edgeKey;
    for (int i = 0; i < faceTab.size(); ++i) {
        ind_vertices = faceTab[i].vertices();
        for (size_t j = 0; j < 3; ++j) {
            // Création de la clef d'une arête (edgeKey)
            first_v = std::min(ind_vertices[j], ind_vertices[(j + 1) % 3]);
            scnd_v = std::max(ind_vertices[j], ind_vertices[(j + 1) % 3]);
            edgeKey = std::to_string(first_v) + "-" + std::to_string(scnd_v);
            if (edges.find(edgeKey) == edges.end()) { // arête non répertoriée
                edges[edgeKey] = i;
            } else {
                // connexion des faces adjacentes
                int adjacentFace = edges[edgeKey];
                // connecter la face adjacente à la face courante
                faceTab[i].addAdjacentFace(adjacentFace, (j + 2) % 3);
                // ajouter la face courante à la face adjacente
                for (size_t k = 0; k < 3; ++k) {
                    if (faceTab[adjacentFace].vertices()[k] != first_v
                            && faceTab[adjacentFace].vertices()[k] != scnd_v) {
                        faceTab[adjacentFace].addAdjacentFace(i, k);
                        break;
                    }
                }
            }
        }
    }
}

Tetrahedron::Tetrahedron() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 1));
    vertexTab.push_back(Vertex(Point(0,0.5,-0.5), 2));
    vertexTab.push_back(Vertex(Point(0,-0.5,0.5), 3));
    // Création des faces
    std::array<int, 3> ind_vertices; // indice des sommets
    std::array<int, 3> ind_faces; // indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {1, 2, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 0
    ind_vertices = {1, 3, 2}; ind_faces = {2, 0, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 1
    ind_vertices = {3, 0, 2}; ind_faces = {0, 1, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 2
    ind_vertices = {0, 3, 1}; ind_faces = {1, 0, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 3
}

Pyramid::Pyramid() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1));
    vertexTab.push_back(Vertex(Point(0,0,0.5), 2));
    // Création des faces
    std::array<int, 3> ind_vertices; // indice des sommets
    std::array<int, 3> ind_faces; // indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {1, 5, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 0
    ind_vertices = {1, 3, 2}; ind_faces = {4, 0, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 1
    ind_vertices = {0, 4, 1}; ind_faces = {3, 0, 5};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 2
    ind_vertices = {1, 4, 3}; ind_faces = {4, 1, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 3
    ind_vertices = {2, 3, 4}; ind_faces = {3, 5, 1};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 4
    ind_vertices = {0, 2, 4}; ind_faces = {4, 2, 0};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 5
}

BoundingBox2D::BoundingBox2D() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1));
    // Création des faces
    std::array<int, 3> ind_vertices; // indice des sommets
    std::array<int, 3> ind_faces; // indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {1, 5, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 0
    ind_vertices = {1, 3, 2}; ind_faces = {4, 0, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 1
    ind_vertices = {0, -1, 1}; ind_faces = {3, 0, 5};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 2
    ind_vertices = {1, -1, 3}; ind_faces = {4, 1, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 3
    ind_vertices = {2, 3, -1}; ind_faces = {3, 5, 1};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 4
    ind_vertices = {0, 2, -1}; ind_faces = {4, 2, 0};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 5
}

QueenMesh::QueenMesh() {

    /**** Lecture du fichier ****/
    std::ifstream offFile("queen.off");
    if (!offFile) { return; } // le fichier ne peut pas s'ouvrir
    std::string line;
    int nbVertices, nbFaces;
    std::getline(offFile, line);
    std::istringstream iss(line);
    iss >> nbVertices >> nbFaces;

    /**** Création des sommets ****/
    float x, y, z;
    int delimiterPos1, delimiterPos2, delimiterPos3;
    for (int i = 0; i < nbVertices; ++i) {
        std::getline(offFile, line);
        delimiterPos1 = line.find(" ", 0);
        x = atof(line.substr(0,delimiterPos1).c_str()); // x
        delimiterPos2 = line.find(" ", delimiterPos1 + 1);
        y = atof(line.substr(delimiterPos1, delimiterPos2).c_str()); // y
        delimiterPos3 = line.find(" ", delimiterPos2 + 1);
        z = atof(line.substr(delimiterPos2, delimiterPos3 ).c_str()); // z
        vertexTab.push_back(Vertex(Point(x, y, z), -1));
    }

    /**** Création des faces ****/
    int nbSommets, v1, v2, v3; // indice des sommets
    std::array<int, 3> ind_vertices, ind_faces; // Tableau des indices des
    //sommets et des faces adjacentes à une face
    for (int i = 0; i < nbFaces; ++i) {
        std::getline(offFile, line);
        iss.str(line);
        iss >> nbSommets >> v1 >> v2 >> v3;
        iss.clear();
        ind_vertices = {v1, v2, v3};
        ind_faces = {-1, -1, -1};
        // Ajouter les faces incidentes au sommets
        if (vertexTab[v1].face() == -1) vertexTab[v1].setFace(i);
        if (vertexTab[v2].face() == -1) vertexTab[v2].setFace(i);
        if (vertexTab[v3].face() == -1) vertexTab[v3].setFace(i);
        faceTab.push_back(Face(ind_vertices, ind_faces));
    }

    // Connecter les faces adjacentes
    connectAdjacentFaces();

    /**** Ajout des triangles fictifs ****/
    for (int i = 0; i < nbFaces; ++i) {
        ind_faces = faceTab[i].adjacentFaces();
        for (size_t j = 0; j < 3; ++j) {
            if (ind_faces[j] == -1) {
                // Créer un nouveau triangle
                faceTab.push_back(Face({faceTab[i].vertices()[(j + 1) % 3],
                                        faceTab[i].vertices()[(j + 2) % 3],
                                        -1}, // sommet fictif
                                        {-1, -1, i}));
                faceTab[i].addAdjacentFace(faceTab.size() -1, j);
            }
        }
    }

    // Ajout des connexions des faces adjacentes entre faces fictives
    connectAdjacentFaces();
}


Iterator_on_faces Mesh::faces_begin() {
    return Iterator_on_faces(faceTab.begin());
}

Iterator_on_faces Mesh::faces_end() {
    return Iterator_on_faces(faceTab.end());
}

Iterator_on_vertices Mesh::vertices_begin() {
    return Iterator_on_vertices(vertexTab.begin());
}

Iterator_on_vertices Mesh::vertices_past_the_end() {
    return Iterator_on_vertices(vertexTab.end());
}

Circulator_on_faces Mesh::incident_faces(Vertex &v) {
    // Récupérer l'index de v
    int ind_v = -1;
    for (int i = 0; i < vertexTab.size(); ++i) {
        if (&v == &(vertexTab[i])) {
            ind_v = i;
            break;
        }
    }
    int first_face = v.face();
    // Trouver les faces incidentes à v.
    std::vector<Face*> faces_incidentes;
    int id_v_oppose, face_actuelle = first_face;
    bool isFaceFictitious;
    while(true) {
        isFaceFictitious = false;
        // Trouver l'indice de v dans la face actuelle.
        for (int i = 0; i < 3; ++i) {
            if (faceTab[face_actuelle].vertices()[i] == -1) isFaceFictitious = true;
            if(faceTab[face_actuelle].vertices()[i] == ind_v) {
                id_v_oppose = (i + 1) % 3;
            }
        }
        if (!isFaceFictitious) {
            faces_incidentes.push_back(&(faceTab[face_actuelle]));
        }
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
        if(face_actuelle == first_face) {
            break;
        }
    }
    Circulator_on_faces cof(faces_incidentes);
    return cof;
}

Circulator_on_vertices Mesh::incident_vertices(Vertex &v) {
    // Récupérer l'index de v
    int ind_v = -1;
    for (int i = 0; i < vertexTab.size(); ++i) {
        if (&v == &(vertexTab[i])) {
            ind_v = i;
            break;
        }
    }
    int first_face = v.face();
    // Trouver les faces incidentes à v.
    std::vector<Vertex*> sommets_adjacents;
    int id_v_oppose, face_actuelle = first_face;
    while(true) {
        // Trouver l'indice de v dans la face actuelle.
        for (int i = 0; i < 3; ++i) {
            if(faceTab[face_actuelle].vertices()[i] == ind_v) {
                id_v_oppose = (i + 1) % 3;
                break;
            }
        }
        if (faceTab[face_actuelle].vertices()[id_v_oppose] != -1)
            sommets_adjacents.push_back(&(vertexTab[faceTab[face_actuelle].vertices()[id_v_oppose]]));
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
        if(face_actuelle == first_face) {
            break;
        }
    }
    Circulator_on_vertices cov(sommets_adjacents);
    return cov;
}
