#include "mesh.h"

Mesh::Mesh() {}
Mesh::~Mesh() {}

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
                faceTab[i].setAdjacentFace(adjacentFace, (j + 2) % 3);
                // ajouter la face courante à la face adjacente
                for (size_t k = 0; k < 3; ++k) {
                    if (faceTab[adjacentFace].vertices()[k] != first_v
                            && faceTab[adjacentFace].vertices()[k] != scnd_v) {
                        faceTab[adjacentFace].setAdjacentFace(i, k);
                        break;
                    }
                }
            }
        }
    }
}

Tetrahedron::Tetrahedron() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0, 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 1, 1));
    vertexTab.push_back(Vertex(Point(0,0.5,-0.5), 2, 2));
    vertexTab.push_back(Vertex(Point(0,-0.5,0.5), 3, 3));
    // Création des faces
    faceTab.push_back(Face({0, 1, 2}, {1, 2, 3}, 0)); // face 0
    faceTab.push_back(Face({1, 3, 2}, {2, 0, 3}, 1)); // face 1
    faceTab.push_back(Face({3, 0, 2}, {0, 1, 3}, 2)); // face 2
    faceTab.push_back(Face({0, 3, 1}, {1, 0, 2}, 3)); // face 3
}

Pyramid::Pyramid() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0, 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0, 1));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0, 2));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1, 3));
    vertexTab.push_back(Vertex(Point(0,0,0.5), 2, 4));
    // Création des faces
    faceTab.push_back(Face({0, 1, 2}, {1, 5, 2}, 0)); // face 0
    faceTab.push_back(Face({1, 3, 2}, {4, 0, 3}, 1)); // face 1
    faceTab.push_back(Face({0, 4, 1}, {3, 0, 5}, 2)); // face 2
    faceTab.push_back(Face({1, 4, 3}, {4, 1, 2}, 3)); // face 3
    faceTab.push_back(Face({2, 3, 4}, {3, 5, 1}, 4)); // face 4
    faceTab.push_back(Face({0, 2, 4}, {4, 2, 0}, 5)); // face 5
}

BoundingBox2D::BoundingBox2D() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0, 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0, 1));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0, 2));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1, 3));
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-100), 2, 4, true)); // Sommet infini
    // Création des faces
    faceTab.push_back(Face({0, 1, 2}, {0, 1, 2}, 0)); // face 0
    faceTab.push_back(Face({1, 3, 2}, {4, 0, 3}, 1)); // face 1
    faceTab.push_back(Face({0, 4, 1}, {3, 0, 5}, 2)); // face 2
    faceTab.push_back(Face({1, 4, 3}, {4, 1, 2}, 3)); // face 3
    faceTab.push_back(Face({2, 3, 4}, {3, 5, 1}, 4)); // face 4
    faceTab.push_back(Face({0, 2, 4}, {4, 2, 0}, 5)); // face 5
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
        vertexTab.push_back(Vertex(Point(x, y, z), -1, i));
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
                faceTab[i].setAdjacentFace(faceTab.size() -1, j);
            }
        }
    }

    // Ajout des connexions des faces adjacentes entre faces fictives
    connectAdjacentFaces();
}

Mesh2D::Mesh2D() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-100,  0, 0), 0, 0));
    vertexTab.push_back(Vertex(Point( 100, -100, 0), 0, 1));
    vertexTab.push_back(Vertex(Point( 100,  100, 0), 0, 2));

    vertexTab.push_back(Vertex(Point(0, 0, -1), 1, 3, true)); // Sommet infini
    _inf_v = 3;

    // Création des faces
    faceTab.push_back(Face({0, 1, 2}, {1, 2, 3}, 0));  // face 0
    faceTab.push_back(Face({1, 3, 2}, {2, 0, 3}, 1));  // face 1
    faceTab.push_back(Face({0, 2, 3}, {1, 3, 0}, 2));  // face 2
    faceTab.push_back(Face({0, 3, 1}, {1, 0, 2}, 3));  // face 3

    this->buildVoronoi();
}

Parabola::Parabola() : Mesh2D() {
    // Changement de profondeur des sommets existants
    for (QVector<Vertex>::iterator vertex_it = vertexTab.begin(); vertex_it != vertexTab.end(); ++vertex_it) {
        if (vertex_it->isFictive()) continue;
        vertex_it->setPoint(phi(vertex_it->point()));
        vertex_it->setDisplay(false);
    }

    double x, y, z;

    //On simule la fonction
    double n = 2000; //precision du maillage
    for(int i = 0; i < n; i++){
        x = -0.5 + (((float) rand()) / (float) RAND_MAX) * 1.0;
        y = -0.5 + (((float) rand()) / (float) RAND_MAX) * 1.0;

        z = pow(x, 2) + pow(y, 2);
        //if (z > 0.5) z = z / 2500.0;
        insertion(Point(x, y, z));
        /*for(int yIndex = 0; yIndex < n; yIndex++){
            double x;
            double y;
            double z;

            // Discretisation de l'espace 2D [-5,5]^2 sur un maillage de taille 1000*1000
            x = -0.5*(1-xIndex/n)+0.5*(xIndex/n);
            y = -0.5*(1-yIndex/n)+0.5*(yIndex/n);

            // Definition de la fonctionelle de R^2 |--> R
            z = x*x+y*y;

            //Ajout du point
            insertion(Point(x, y, z));
        }*/
    }
    // Attribution des couleurs
    // trouver zmin et zmax
    double zmin = DBL_MAX, zmax = 0.0, hue;
    for (QVector<Vertex>::iterator vertex_it = vertexTab.begin(); vertex_it != vertexTab.end(); ++vertex_it) {
        if (vertex_it->isFictive() || !vertex_it->isVisible()) continue;
        zmin = std::min(zmin, vertex_it->point().z());
        zmax = std::max(zmax, vertex_it->point().z());
    }
    for (QVector<Vertex>::iterator vertex_it = vertexTab.begin(); vertex_it != vertexTab.end(); ++vertex_it) {
        // Courbure faible : vert, à courbure forte : rouge
        hue = ((vertex_it->point().z() - zmin) / zmax) * 270.0 + 90.0;
        vertex_it->setColor(hsv2rgb(hue, 1.0, 1.0));
    }
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
    // Trouver les faces incidentes à v
    int first_face = v.face();
    std::vector<Face*> faces_incidentes;
    int id_v_oppose, face_actuelle = first_face;
    do { // Tant qu'on n'a pas faire le tour du sommet v, on accumule les faces incidentes.
        // Trouver l'indice de v dans la face actuelle.
        for (int i = 0; i < 3; ++i) {
            if (faceTab[face_actuelle].vertices()[i] == v.idx()) {
                id_v_oppose = (i + 1) % 3;
            }
        }
        faces_incidentes.push_back(&(faceTab[face_actuelle]));
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
    }
    while(face_actuelle != first_face);

    Circulator_on_faces cof(faces_incidentes);
    return cof;
}

Circulator_on_faces Mesh::incident_faces(Vertex &v, int face_start) {
    Circulator_on_faces cof(incident_faces(v)), cfbegin;
    cfbegin = cof;
    // Commencer avec la face face_start
    do {
        if (cof->idx() == face_start) return cof;
        ++cof;
    } while(cof != cfbegin);
    return cof;
}

Circulator_on_vertices Mesh::neighbour_vertices(Vertex &v) {
    // Trouver les sommets voisins de v
    int first_face = v.face();
    std::vector<Vertex*> sommets_voisins;
    int id_v_oppose, face_actuelle = first_face;
    do {
        // Trouver l'indice de v dans la face actuelle.
        for (int i = 0; i < 3; ++i) {
            if(faceTab[face_actuelle].vertices()[i] == v.idx()) {
                id_v_oppose = (i + 1) % 3;
                break;
            }
        }
        sommets_voisins.push_back(&(vertexTab[faceTab[face_actuelle].vertices()[id_v_oppose]]));
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
    }
    while(face_actuelle != first_face);

    Circulator_on_vertices cov(sommets_voisins);
    return cov;
}


std::ostream& operator<<(std::ostream &strm, const Mesh &m) {
    strm << "\nVertices:\n";
    for (int i = 0; i < m.vertexTab.size(); ++i) {
        strm << i << ": " << m.vertexTab[i].point().x() << " "
                          << m.vertexTab[i].point().y() << " "
                          << m.vertexTab[i].point().z() << std::endl;
    }
    strm << "Faces:\n";
    for (int i = 0; i < m.faceTab.size(); ++i) {
        strm << i << ":\n";
        strm << "   v: " << m.faceTab[i].vertices()[0] << " "
                        << m.faceTab[i].vertices()[1] << " "
                        << m.faceTab[i].vertices()[2] << std::endl;
        strm << "   a: " << m.faceTab[i].adjacentFaces()[0] << " "
                        << m.faceTab[i].adjacentFaces()[1] << " "
                        << m.faceTab[i].adjacentFaces()[2] << std::endl;
    }
    return strm;
}
