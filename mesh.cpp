#include "mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>

void Face::addAdjacentFace(int faceIndex, int pos) {
    _adjacentFaces[pos] = faceIndex;
}

Mesh::Mesh() {}
Mesh::~Mesh() {}

void Mesh::computeLaplacians() {
    float A; // Aire
    Point a, b, c; // Vecteur du triangle pour les calculs

    // On utilise l'itérateur sur les vertices pour parcourir tous les vertex
    Iterator_on_vertices it_v;
    Circulator_on_faces cf, cfbegin;
    Circulator_on_vertices cv, cvbegin;
    std::vector<double> alpha; // tableau des angles alpha pour le laplacien
    std::vector<double> beta; // tableau des angles beta
    size_t alpha_idx, beta_idx, i_idx;
    double lap_x, lap_y, lap_z;
    int i = -1;
    for (it_v = this->vertices_begin(); it_v !=this->vertices_past_the_end(); it_v++) { // on parcourt tous les sommets du mesh
        i++;
        cfbegin = this->incident_faces(*it_v);
        A = 0;
        cf = cfbegin;
        alpha.clear();
        beta.clear();
        // On parcourt toutes les faces qui ont le sommet it_v
        do {
            // Calcul de l'aire
            a = vertexTab[cf->vertices()[0]].point();
            b = vertexTab[cf->vertices()[1]].point();
            c = vertexTab[cf->vertices()[2]].point();
            A = A + (1.f / 3.f) * ((1.f / 2.f) * (crossProduct(difference(b, a), difference(c, a))).norm());
            // On recueille les angles alpha et beta
            // Trouver l'indice du sommet i sur la face
            for (size_t i = 0; i < 3; ++i) {
                if (it_v == &(vertexTab[cf->vertices()[i]])) {
                    i_idx = i;
                    break;
                }
            }
            alpha.push_back(findAngle(vertexTab[cf->vertices()[i_idx]].point(),
                                      vertexTab[cf->vertices()[(i_idx + 1) % 3]].point(),
                                      vertexTab[cf->vertices()[(i_idx + 2) % 3]].point()));
            beta.push_back(findAngle(vertexTab[cf->vertices()[(i_idx + 1) % 3]].point(),
                                     vertexTab[cf->vertices()[(i_idx + 2) % 3]].point(),
                                     vertexTab[cf->vertices()[i_idx]].point()));
            cf++;
        } while (cf != cfbegin);

        lap_x = 0;
        lap_y = 0;
        lap_z = 0;
        alpha_idx = alpha.size() - 1;
        beta_idx = 0;
        int idx = 0;
        cvbegin = this->incident_vertices(*it_v);
        cv = cvbegin;
        do {
            lap_x += (1.f / std::tan(alpha[alpha_idx]) + 1.f / std::tan(beta[beta_idx])) * difference(cv->point(), it_v->point()).x();
            lap_y += (1.f / std::tan(alpha[alpha_idx]) + 1.f / std::tan(beta[beta_idx])) * difference(cv->point(), it_v->point()).y();
            lap_z += (1.f / std::tan(alpha[alpha_idx]) + 1.f / std::tan(beta[beta_idx])) * difference(cv->point(), it_v->point()).z();
            alpha_idx = (alpha_idx + 1) % alpha.size();
            beta_idx = (beta_idx + 1) % beta.size();
            idx++;
            cv++;
        } while (cv != cvbegin);
        // Calcul du Laplacien pour le sommet it_v;
        lap_x = (1.f / (2.f * A)) * lap_x;
        lap_y = (1.f / (2.f * A)) * lap_y;
        lap_z = (1.f / (2.f * A)) * lap_z;
        it_v->setLaplacian(Point(lap_x, lap_y, lap_z));
    }
}

void Mesh::computeCurvature() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {

        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        if (faceVertices[0] == -1 || faceVertices[1] == -1 || faceVertices[2] == -1) continue;
        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];
        double norm_l_a = a.laplacian().norm();
        double norm_l_b = b.laplacian().norm();
        double norm_l_c = c.laplacian().norm();
        Point a_l_n = Point(a.laplacian().x() / norm_l_a, a.laplacian().y() / norm_l_a, a.laplacian().z() / norm_l_a);
        Point b_l_n = Point(b.laplacian().x() / norm_l_b, b.laplacian().y() / norm_l_b, b.laplacian().z() / norm_l_b);
        Point c_l_n = Point(c.laplacian().x() / norm_l_c, c.laplacian().y() / norm_l_c, c.laplacian().z() / norm_l_c);
        Point laplacian_face((a_l_n.x() + b_l_n.x() + c_l_n.x()) / 3,
                             (a_l_n.y() + b_l_n.y() + c_l_n.y()) / 3,
                             (a_l_n.z() + b_l_n.z() + c_l_n.z()) / 3);
        face_it->setCurvature(laplacian_face.norm() / 2.f);
    }
}

void Mesh::computeColors() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        face_it->setColor(hsv2rgb(face_it->curvature() * 255, 255, 255));
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
        if (id_v_oppose != -1) {
            sommets_adjacents.push_back(&(vertexTab[id_v_oppose]));
        }
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
        if(face_actuelle == first_face) {
            break;
        }
    }
    Circulator_on_vertices cov(sommets_adjacents);
    return cov;
}
