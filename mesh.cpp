#include "mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>

Mesh::Mesh() {}

Mesh::~Mesh() {}

QVector<QVector<double>> Mesh::getLap() {
    QVector<QVector<double>> answ; //Les valeurs du lapacien
    double x,y,z; //Coordones du laplacien
    double A; // Aire
    Point a,b,c,d; //Vector du triangle pour les calculs
    double s_x,s_y,s_z; //Somme sur les vertices adjacents pour chacune des coordone du laplacien
    double opp,adj,cot_alpha,cot_beta; //Calcul de trigo

    //On utilise l'iterateur sur les verticies pour parcourir tous les vertex
    Iterator_on_vertices its;
    Circulator_on_faces cf;
    for (its=this->vertices_begin(); its !=this->vertices_past_the_end(); ++its){ //On parcour tous les vertex du mesh
        //On utilise le circulateur sur les faces pour recuperer un tableau des sommets adjacents
        Circulator_on_faces cfbegin=this->incident_faces(*its) ;

        //Calcul de l'aire
        A=0;
        for (cf=cfbegin,++cf; cf!=cfbegin; cf++){ //On parcours toutes les faces qui ont le sommet its
            a = vertexTab[cf->vertices()[0]].point();
            b = vertexTab[cf->vertices()[1]].point();
            c = vertexTab[cf->vertices()[2]].point();
            A=A+1.f/3.f*(1.f/2.f*(produitVectoriel(b.difference(a), c.difference(a))).norm());
        }
        cf=cfbegin;
        a = vertexTab[cf->vertices()[0]].point();
        b = vertexTab[cf->vertices()[1]].point();
        c = vertexTab[cf->vertices()[2]].point();
        A=A+1/3*(1/2*(produitVectoriel(b.difference(a), c.difference(a))).norm());

        //Calcul de la somme
        s_x=s_y=s_z=0;
        Circulator_on_vertices cv;
        Circulator_on_vertices cvbegin=this->incident_vertices(*its);
        int index; //0 est le cvbegin
        index=1;
        a=its->point();
        for (cv=cvbegin,cv++; cv!=cvbegin; cv++){

            b=cv->point();
            c=(cv++)->point();
            d=(cv++)->point();
            cv=cvbegin;
            for(int i=0; i<index; i++){
                cv++;
            }
            index++;
            // cot_alpha
            opp=(a.difference(c)).norm();
            adj=(a.difference(b)).norm();
            cot_alpha=adj/opp;

            // cot_beta
            adj=(a.difference(d)).norm();
            cot_beta=adj/opp;

            // sommes
            s_x=s_x+cot_alpha+cot_beta*(c.x()-a.x());
            s_y=s_y+cot_alpha+cot_beta*(c.y()-a.y());
            s_z=s_z+cot_alpha+cot_beta*(c.z()-a.z());
        }

        cv=cvbegin;
        b=cv->point();
        c=(cv++)->point();
        d=(cv++)->point();
        // cot_alpha
        opp=(a.difference(c)).norm();
        adj=(a.difference(b)).norm();
        cot_alpha=adj/opp;
        // cot_beta
        adj=(a.difference(d)).norm();
        cot_beta=adj/opp;

        // sommes
        s_x=s_x+cot_alpha+cot_beta*(c.x()-a.x());
        s_y=s_y+cot_alpha+cot_beta*(c.y()-a.y());
        s_z=s_z+cot_alpha+cot_beta*(c.z()-a.z());

        x=1/(2*A)*s_x;
        y=1/(2*A)*s_y;
        z=1/(2*A)*s_z;

        QVector<double> tmp; //Valeur temporaire du laplacien pour ajouter a answ
        tmp.clear();
        tmp.push_back(x);
        tmp.push_back(y);
        tmp.push_back(z);
        answ.push_back(tmp);

    }
    std::cout<<answ.size()<<std::endl;
    return answ;
}

Tetrahedron::Tetrahedron() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 1));
    vertexTab.push_back(Vertex(Point(0,0.5,-0.5), 2));
    vertexTab.push_back(Vertex(Point(0,-0.5,0.5), 3));
    // Création des faces
    std::array<int, 3> ind_vertices; // Indice des sommets
    std::array<int, 3> ind_faces; // Indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {1, 2, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 0
    ind_vertices = {1, 3, 2}; ind_faces = {2, 0, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 1
    ind_vertices = {3, 0, 2}; ind_faces = {0, 1, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 2
    ind_vertices = {0, 3, 1}; ind_faces = {1, 0, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 3
}

Pyramid::Pyramid() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1));
    vertexTab.push_back(Vertex(Point(0,0,0.5), 2));
    // Création des faces
    std::array<int, 3> ind_vertices; // Indice des sommets
    std::array<int, 3> ind_faces; // Indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {1, 5, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 0
    ind_vertices = {1, 3, 2}; ind_faces = {4, 0, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 1
    ind_vertices = {0, 4, 1}; ind_faces = {3, 0, 5};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 2
    ind_vertices = {1, 4, 3}; ind_faces = {4, 1, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 3
    ind_vertices = {2, 3, 4}; ind_faces = {3, 5, 1};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 4
    ind_vertices = {0, 2, 4}; ind_faces = {4, 2, 0};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 5
}

BoundingBox2D::BoundingBox2D() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,-0.5), 0));
    vertexTab.push_back(Vertex(Point(0.5,0.5,-0.5), 1));
    // Création des faces
    std::array<int, 3> ind_vertices; // Indice des sommets
    std::array<int, 3> ind_faces; // Indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {-1, 5, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 0
    ind_vertices = {1, 3, 2}; ind_faces = {4, -1, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 1
    ind_vertices = {0, -1, 1}; ind_faces = {3, 0, 5};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 2
    ind_vertices = {1, -1, 3}; ind_faces = {4, 1, 2};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 3
    ind_vertices = {2, 3, -1}; ind_faces = {3, 5, 1};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 4
    ind_vertices = {0, 2, -1}; ind_faces = {4, 2, 0};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // Face 5
}

QueenMesh::QueenMesh() {
    // Lecture du fichier
    std::ifstream offFile("queen.off");
    if (!offFile) { return; } // Failed to open file
    std::string line;
    int nbVertices, nbFaces;
    std::getline(offFile, line);
    std::istringstream iss(line);
    iss >> nbVertices >> nbFaces;
    // Création des sommets
    float x, y, z;
    int delimiterPos1, delimiterPos2, delimiterPos3;
    for (int i = 0; i < nbVertices; ++i) {
        std::getline(offFile, line);
        delimiterPos1 = line.find(" ", 0);
        x = atof(line.substr(0,delimiterPos1).c_str());
        delimiterPos2 = line.find(" ", delimiterPos1 + 1);
        y = atof(line.substr(delimiterPos1, delimiterPos2).c_str());
        delimiterPos3 = line.find(" ", delimiterPos2 + 1);
        z = atof(line.substr(delimiterPos2, delimiterPos3 ).c_str());
        vertexTab.push_back(Vertex(Point(x, y, z), -1));
    }
    // Création des faces
    int nbSommets, v1, v2, v3; // Indice des sommets
    std::unordered_map<std::string, int> edges;
    std::array<int, 3> ind_vertices, ind_faces;
    int first_v, scnd_v;
    for (int i = 0; i < nbFaces; ++i) {
        std::getline(offFile, line);
        iss.str(line);
        iss >> nbSommets >> v1 >> v2 >> v3;
        iss.clear();
        ind_vertices = {v1, v2, v3};
        ind_faces = {-1, -1, -1};
        // Add face to vertices
        if (vertexTab[v1].face() == -1) vertexTab[v1].setFace(i);
        if (vertexTab[v2].face() == -1) vertexTab[v2].setFace(i);
        if (vertexTab[v3].face() == -1) vertexTab[v3].setFace(i);
        faceTab.push_back(Face(ind_vertices, ind_faces));
        for (int j = 0; j < 3; ++j) {
            first_v = std::min(ind_vertices[j], ind_vertices[(j + 1) % 3]);
            scnd_v = std::max(ind_vertices[j], ind_vertices[(j + 1) % 3]);
            if (edges.find(std::to_string(first_v) + "-" + std::to_string(scnd_v)) == edges.end()) { // Arete non répertoriée
                edges[std::to_string(first_v) + "-" + std::to_string(scnd_v)] = i;
            } else {
                // Add adjacent face.
                int adjacentFace = edges[std::to_string(first_v) + "-" + std::to_string(scnd_v)];
                faceTab[i].addAdjacentFace(adjacentFace, (j + 2) % 3);
                for (int k = 0; k < 3; ++k) {
                    if (faceTab[adjacentFace].vertices()[k] != first_v && faceTab[adjacentFace].vertices()[k] != scnd_v) {
                        faceTab[adjacentFace].addAdjacentFace(i, k);
                        break;
                    }
                }

            }
        }
        first_v = std::min(v1, v2);
        scnd_v = std::max(v1, v2);

   }
    // Ajout des triangles fictifs
    for (int i = 0; i < nbFaces; ++i) {
        ind_faces = faceTab[i].adjacentFaces();
        for (int j = 0; j < 3; ++j) {
            if (ind_faces[j] == -1) {
                // Créer un nouveau triangle
                faceTab.push_back(Face({faceTab[i].vertices()[(j + 1) % 3], faceTab[i].vertices()[(j + 2) % 3], -1}, {-1, -1, i}));
                faceTab[i].addAdjacentFace(faceTab.size() -1, j);
            }
        }
    }
    // Ajout des connexions des faces adjacentes entre faces fictives
    for (int i = nbFaces; i < faceTab.size(); ++i) {
        ind_vertices = faceTab[i].vertices();
        for (int j = 0; j < 3; ++j) {
            first_v = std::min(ind_vertices[j], ind_vertices[(j + 1) % 3]);
            scnd_v = std::max(ind_vertices[j], ind_vertices[(j + 1) % 3]);
            if (edges.find(std::to_string(first_v) + "-" + std::to_string(scnd_v)) == edges.end()) { // Arete non répertoriée
                edges[std::to_string(first_v) + "-" + std::to_string(scnd_v)] = i;
            } else {
                // Add adjacent face.
                int adjacentFace = edges[std::to_string(scnd_v) + "-" + std::to_string(scnd_v)];
                faceTab[i].addAdjacentFace(adjacentFace, (j + 2) % 3);
                for (int k = 0; k < 3; ++k) {
                    if (faceTab[adjacentFace].vertices()[k] != first_v && faceTab[adjacentFace].vertices()[k] != scnd_v) {
                        faceTab[adjacentFace].addAdjacentFace(i, k);
                        break;
                    }
                }
            }
        }
   }
    offFile.close();
}


// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Vertex & v) {
    const Point p = v.point();
    glVertex3f(p.x(), p.y(), p.z());
}

//Example with a tetraedra
void Mesh::drawMesh() {
    QVector<QVector<double>> lap;
    lap=getLap();
    int color_ind = 0;
    // Iteration sur chaque face du mesh
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {

        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        if (faceVertices[0] == -1 || faceVertices[1] == -1 || faceVertices[2] == -1) continue;
        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];
        // Couleur
        /*
        switch(color_ind % 6) {
            case 0: glColor3d(1,0,0); break; // Red
            case 1: glColor3d(0,1,0); break; // Green
            case 2: glColor3d(0,0,1); break; // Blue
            case 3: glColor3d(0.8, 0, 0.8); break;
            case 4: glColor3d(0.4, 0.2, 0.8); break;
            default: glColor3d(1,1,0); break; // Yellow
        }
        */
        //Utilisation de la courbure pour definir une couleur
        glColor3d(lap[color_ind][1], lap[color_ind][1], lap[color_ind][1]);
        // Création du triangle
        glBegin(GL_TRIANGLES);
        glVertexDraw(a); // 1er point de la face
        glVertexDraw(b); // 2e point de la face
        glVertexDraw(c); // 3e point de la face
        glEnd();

        color_ind++;
    }
}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        if (faceVertices[0] == -1 || faceVertices[1] == -1 || faceVertices[2] == -1) continue;
        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];
        glBegin(GL_LINE_STRIP);
        glVertexDraw(a);
        glVertexDraw(b);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertexDraw(b);
        glVertexDraw(c);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertexDraw(c);
        glVertexDraw(a);
        glEnd();
    }
}

Iterator_on_faces Mesh::faces_begin() {
    return Iterator_on_faces(faceTab.begin());
}

Iterator_on_faces Mesh::faces_end() {
    return Iterator_on_faces(faceTab.end());
}

bool Iterator_on_faces::operator!=(const Iterator_on_faces& it) const {
    return p != it.p;
}

Iterator_on_vertices Mesh::vertices_begin() {
    return Iterator_on_vertices(vertexTab.begin());
}

Iterator_on_vertices Mesh::vertices_past_the_end() {
    return Iterator_on_vertices(vertexTab.end());
}

bool Iterator_on_vertices::operator!=(const Iterator_on_vertices& it) const {
    return p != it.p;
}

bool Circulator_on_faces::operator!=(const Circulator_on_faces& it) const {
    return faces_adjacentes != it.faces_adjacentes || ind != it.ind;
}

bool Circulator_on_vertices::operator!=(const Circulator_on_vertices& it) const {
    return sommets_adjacents != it.sommets_adjacents || ind != it.ind;
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
    // Trouver les faces incidentes de v.
    std::vector<Face*> faces_incidentes;
    faces_incidentes.push_back(&faceTab[first_face]);
    int id_v_oppose, face_actuelle = v.face();
    while(true) {
        // Trouver l'indice de v dans la face actuelle.
        id_v_oppose = -2;
        for (int i = 0; i < 3; ++i) {
            if(faceTab[face_actuelle].vertices()[i] == ind_v) {
                id_v_oppose = (i + 1) % 3;
                break;
            }
        }
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
        if(face_actuelle == first_face) {
            break;
        }
        else {
            faces_incidentes.push_back(&(faceTab[face_actuelle]));
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
    // Trouver les faces incidentes de v.
    std::vector<Vertex*> sommets_adjacents;
    int id_v_oppose, face_actuelle = v.face();
    while(true) {
        // Trouver l'indice de v dans la face actuelle.
        for (int i = 0; i < 3; ++i) {
            if(faceTab[face_actuelle].vertices()[i] == ind_v) {
                id_v_oppose = (i + 1) % 3;
                break;
            }
        }
        face_actuelle = faceTab[face_actuelle].adjacentFaces()[id_v_oppose];
        if(face_actuelle == first_face) {
            break;
        } else {
            sommets_adjacents.push_back(&(vertexTab[id_v_oppose]));
        }
    }
    Circulator_on_vertices cof(sommets_adjacents);
    return cof;
}
