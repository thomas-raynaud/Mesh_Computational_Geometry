#include "mesh.h"

Mesh::Mesh() {}
Mesh::~Mesh() {}

std::vector<Point> Mesh::getLaplacians() {
    if (laplacianTab.size() != 0) return laplacianTab;
    float A; // Aire
    Point a, b, c, d; // Points adjacents au sommet traité.
    Iterator_on_vertices it_v;
    Circulator_on_faces cf, cfbegin;
    Circulator_on_vertices cv, cvbegin, cvtemp;
    double lap_x, lap_y, lap_z;
    double opp, adj, cot_alpha, cot_beta, angles; // Calculs de trigo

    // Parcours de tous les sommets du mesh
    for (it_v = this->vertices_begin(); it_v !=this->vertices_past_the_end(); ++it_v) {

        // Calcul de de l'aire
        // On parcourt toutes les faces qui ont le sommet it_v
        cfbegin = this->incident_faces(*it_v);
        cf = cfbegin;
        A = 0;

        do {
            a = vertexTab[cf->vertices()[0]].point();
            b = vertexTab[cf->vertices()[1]].point();
            c = vertexTab[cf->vertices()[2]].point();
            A += (1.f / 3.f) * ((1.f / 2.f) * (crossProduct(difference(b, a), difference(c, a))).norm());
            cf++;
        } while (cf != cfbegin);

        // Calcul du laplacien
        // On parcourt toutes les arêtes du sommet it_v
        lap_x = lap_y = lap_z = 0;
        cvbegin = this->neighbour_vertices(*it_v);
        cv = cvbegin;
        a = it_v->point();
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
            angles = cot_alpha + cot_beta;
            lap_x += angles * (c.x() - a.x());
            lap_y += angles * (c.y() - a.y());
            lap_z += angles * (c.z() - a.z());

            cv++;
        } while (cv != cvbegin);

        // Calcul du Laplacien pour le sommet it_v;
        A = 1.f / (2.f * A);
        lap_x = A * lap_x;
        lap_y = A * lap_y;
        lap_z = A * lap_z;
        laplacianTab.push_back(Point(lap_x, lap_y, lap_z));
    }
    return laplacianTab;
}

void Mesh::computeColors(int curveAxis) {
    double min = DBL_MAX, max = 0.0, mean_curvature;
    int hue = 0;
    std::vector<double> curvature;
    std::vector<Point> laplacians = getLaplacians();
    // Calculer la courbure moyenne
    for (int i = 0; i < vertexTab.size(); ++i) {
        if (curveAxis == 0)
            mean_curvature = std::abs(std::log(laplacians[i].norm()) / -2);
        else
            mean_curvature = std::abs((laplacians[i][curveAxis - 1] / laplacians[i].norm()) / -2);
        min = std::min(min, mean_curvature);
        max = std::max(max, mean_curvature);
        curvature.push_back(mean_curvature);
    }

    // Trouver la teinte de la couleur en fonction de la courbure
    for (int i = 0; i < vertexTab.size(); ++i) {
        mean_curvature = curvature[i];
        // Courbure faible : vert, à courbure forte : rouge
        hue = ((mean_curvature - min) / max) * 270.0 + 90.0;
        vertexTab[i].setColor(hsv2rgb(hue, 1.0, 1.0));
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

void flipEdge(const int &f1, const int &f2) {

}

Tetrahedron::Tetrahedron() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0, 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 1, 1));
    vertexTab.push_back(Vertex(Point(0,0.5,-0.5), 2, 2));
    vertexTab.push_back(Vertex(Point(0,-0.5,0.5), 3, 3));
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
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0, 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0, 1));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0, 2));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1, 3));
    vertexTab.push_back(Vertex(Point(0,0,0.5), 2, 4));
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
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0, 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0, 1));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0, 2));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1, 3));
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
    // Trouver les faces incidentes à v
    int first_face = v.face();
    std::vector<Face*> faces_incidentes;
    int id_v_oppose, face_actuelle = first_face;
    bool isFaceFictitious;
    do { // Tant qu'on n'a pas faire le tour du sommet v, on accumule les faces incidentes.
        isFaceFictitious = false;
        // Trouver l'indice de v dans la face actuelle.
        for (int i = 0; i < 3; ++i) {
            if (faceTab[face_actuelle].vertices()[i] == -1) isFaceFictitious = true;
            if (faceTab[face_actuelle].vertices()[i] == v.idx()) {
                id_v_oppose = (i + 1) % 3;
            }
        }
        if (!isFaceFictitious) {
            faces_incidentes.push_back(&(faceTab[face_actuelle]));
        }
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
    }
    while(face_actuelle != first_face);

    Circulator_on_faces cof(faces_incidentes);
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
        if (faceTab[face_actuelle].vertices()[id_v_oppose] != -1)
            sommets_voisins.push_back(&(vertexTab[faceTab[face_actuelle].vertices()[id_v_oppose]]));
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
    }
    while(face_actuelle != first_face);

    Circulator_on_vertices cov(sommets_voisins);
    return cov;
}
