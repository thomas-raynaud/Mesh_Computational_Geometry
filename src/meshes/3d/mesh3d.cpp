#include "mesh3d.h"

#include <fstream>
#include <sstream>
#include <iostream>


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
    std::setlocale(LC_NUMERIC, "C");
    std::ifstream offFile("resources/queen.off");
    if (!offFile) {
        std::cerr << "Cannot open queen.off file." << std::endl;
        return;
    }
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
        vertexTab[vertexTab.size() - 1].setIdx(i);
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
        faceTab[faceTab.size() - 1].setIdx(i);
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
