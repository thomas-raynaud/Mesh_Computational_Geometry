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
