#include "mesh2d.h"

#include <queue>
#include <iostream>


Mesh2D::Mesh2D() {
    // Create an invisible tetrahedron that wraps up the 2D mesh.
    double far_dist = 20.0;
    m_vertices.push_back(Vertex(Point(-far_dist,  0,        0), 0, 0));
    m_vertices.push_back(Vertex(Point( far_dist, -far_dist, 0), 0, 1));
    m_vertices.push_back(Vertex(Point( far_dist,  far_dist, 0), 0, 2));
    m_vertices.push_back(Vertex(Point(0, 0, -1), 1, 3)); // Infinite vertex
    _inf_v = 3;
    m_hidden_vertices.insert({ 0, 1, 2, 3 });
    faceTab.push_back(Face({0, 1, 2}, {1, 2, 3}, 0));  // face 0
    faceTab.push_back(Face({1, 3, 2}, {2, 0, 3}, 1));  // face 1
    faceTab.push_back(Face({0, 2, 3}, {1, 3, 0}, 2));  // face 2
    faceTab.push_back(Face({0, 3, 1}, {1, 0, 2}, 3));  // face 3

    // Create 3 visible vertices, our actual 2D mesh.
    insertion(Point(-1, -1, 0));
    insertion(Point(1, -1, 0));
    insertion(Point(0, 1, 0));

    this->buildVoronoi();

    std::cout << *this << std::endl;
}

void Mesh2D::flipEdge(const int &f1, const int &f2) {
    int v00, v01; // Sommets de f1 tel que v00 opposé à f2
    int v10, v11; // Sommets de f2 tel que v10 opposé à f1
    int f01, f02, f11, f12; // faces opposées resp. à v01, v02, v11, v12
    int ind_v00 = 0, ind_v10 = 0;
    for (int i = 0; i < 3; ++i) {
        if (faceTab[f1].adjacentFaces()[i] == f2) ind_v00 = i;
        if (faceTab[f2].adjacentFaces()[i] == f1) ind_v10 = i;
    }
    // Sommets de f1
    v00 = faceTab[f1].vertices()[ind_v00];
    v01 = faceTab[f1].vertices()[(ind_v00 + 1) % 3];
    // Sommets de f2
    v10 = faceTab[f2].vertices()[ind_v10];
    v11 = faceTab[f2].vertices()[(ind_v10 + 1) % 3];
    // Faces opposées
    f01 = faceTab[f1].adjacentFaces()[(ind_v00 + 1) % 3];
    f02 = faceTab[f1].adjacentFaces()[(ind_v00 + 2) % 3];
    f11 = faceTab[f2].adjacentFaces()[(ind_v10 + 1) % 3];
    f12 = faceTab[f2].adjacentFaces()[(ind_v10 + 2) % 3];

    // Changer les sommets de f1 et f2
    faceTab[f1].setVertices({v00, v10, v11});
    faceTab[f2].setVertices({v00, v01, v10});
    // Changer les faces adjacentes
    faceTab[f1].setAdjacentFaces({f12, f01, f2});
    faceTab[f2].setAdjacentFaces({f11, f1, f02});
    // Changer les connexions des sommets aux faces
    m_vertices[v00].setFace(f1);
    m_vertices[v01].setFace(f2);
    m_vertices[v10].setFace(f2);
    m_vertices[v11].setFace(f1);
    // Changer les faces adjacentes des faces adjacentes (f02 et f12)
    for (int i = 0; i < 3; ++i) if (faceTab[f02].adjacentFaces()[i] == f1) faceTab[f02].setAdjacentFace(f2, i);
    for (int i = 0; i < 3; ++i) if (faceTab[f12].adjacentFaces()[i] == f2) faceTab[f12].setAdjacentFace(f1, i);
}

void Mesh2D::splitTriangle(int vertexIndex, int faceIndex){
    //Récuperation des attributs de la face à splitter
    std::array<int, 3> verticesOfFace;
    verticesOfFace=this->faceTab[faceIndex].vertices();
    std::array<int, 3> adjacentFaces;
    adjacentFaces=this->faceTab[faceIndex].adjacentFaces();

    //Indices des deux nouvelles faces
    int faceAIndex=this->faceTab.size();
    int faceBIndex=this->faceTab.size()+1;

    //Set faceA
    std::array<int, 3> vertexTmp;
    vertexTmp = {vertexIndex, verticesOfFace[0], verticesOfFace[1]};
    std::array<int, 3> facesTmp;
    facesTmp = {adjacentFaces[2], faceBIndex, faceIndex};
    Face faceA = Face(vertexTmp, facesTmp, faceAIndex);
    this->faceTab.push_back(faceA);

    //Set faceB
    vertexTmp = {vertexIndex, verticesOfFace[1], verticesOfFace[2]};
    facesTmp = {adjacentFaces[0], faceIndex, faceAIndex};
    Face faceB = Face(vertexTmp, facesTmp, faceBIndex);
    this->faceTab.push_back(faceB);

    //Modification de la face splité
    this->faceTab[faceIndex].setVertices(std::array<int,3>{vertexIndex, verticesOfFace[2], verticesOfFace[0]});
    this->faceTab[faceIndex].setAdjacentFaces(std::array<int,3>{adjacentFaces[1], faceAIndex, faceBIndex});

    //Modfication de la face incidente à A
    //Recuperation de l'indice local du sommet opposé à la face A dans la face incidente à A
    vertexTmp = this->faceTab[adjacentFaces[2]].vertices();
    for(int i = 0; i<3; i++){
        if(vertexTmp[i] != verticesOfFace[0] && vertexTmp[i] != verticesOfFace[1]){
            this->faceTab[adjacentFaces[2]].setAdjacentFace(faceAIndex, i);
            break;
        }
    }

    //Modification de la face incidente à B
    //Recuperation de l'indice local du sommet opposé à la face B dans la face incidente à B
    vertexTmp = this->faceTab[adjacentFaces[0]].vertices();
    for(int i = 0; i<3; i++){
        if(vertexTmp[i] != verticesOfFace[1] && vertexTmp[i] != verticesOfFace[2]){
            this->faceTab[adjacentFaces[0]].setAdjacentFace(faceBIndex, i);
            break;
        }
    }

    //Correction de tous les vertex qui doivent avoir une face incidente
    //0 -> A ; 1 -> B ; 2 -> face splité ; nouveau vertex -> face A
    this->m_vertices[verticesOfFace[0]].setFace(faceAIndex);
    this->m_vertices[verticesOfFace[1]].setFace(faceBIndex);
    this->m_vertices[verticesOfFace[2]].setFace(faceIndex);
    this->m_vertices[vertexIndex].setFace(faceAIndex);
}

void Mesh2D::insertion(Point p) {
    // Ajouter le point p dans le sac de sommets
    m_vertices.push_back(Vertex(p, -1, m_vertices.size()));
    // Regarder si p est à l'extérieur de l'enveloppe convexe
    Point a, b, c;
    int splitFace = -1, new_vertex = m_vertices.size() - 1, previousFace, face_start;
    bool edgeVisible, foundFace;
    Circulator_on_faces cf, cfbegin;
    cfbegin = this->incident_faces(m_vertices[_inf_v]); // Circulateur des faces du point infini
    cf = cfbegin;
    do {
        for(int i = 0; i < 3; ++i) {
            if (cf->vertices()[(i + 2) % 3] != _inf_v && cf->vertices()[i] != _inf_v) {
                a = m_vertices[cf->vertices()[(i + 2) % 3]].point();
                b = m_vertices[cf->vertices()[i]].point();
            }
        }
        if (testOrientation(a, b, p) > 0.0) { // Arête visible depuis p
            splitFace = cf->idx();
        }
        ++cf;
    } while(cf != cfbegin && splitFace == -1);
    if (splitFace != -1) {
        splitTriangle(new_vertex, splitFace);
        // Flip les arêtes fictives si incidentes à une autre face fictive
        cfbegin = incident_faces(m_vertices[_inf_v]);
        cf = cfbegin;
        do {
            for (int i = 0; i < 3; ++i) {
                if (cf->vertices()[i] == new_vertex && cf->vertices()[i + 1] == _inf_v) {
                    foundFace = true;
                    face_start = cf->idx();
                }
            }
            if (foundFace) break;
            ++cf;
        } while(cf != cfbegin);
        // Ajouter les arêtes visibles après le triangle split
        edgeVisible = true;
        previousFace = (++cf)->idx();
        ++cf;
        while (edgeVisible == true) {
            for(int i = 0; i < 3; ++i) {
                if (cf->vertices()[(i + 2) % 3] != _inf_v && cf->vertices()[i] != _inf_v) {
                    a = m_vertices[cf->vertices()[(i + 2) % 3]].point();
                    b = m_vertices[cf->vertices()[i]].point();
                }
            }
            if (testOrientation(a, b, p) > 0.0) { // Arête visible depuis p
                // Flip l'arete
                flipEdge(cf->idx(), previousFace);
                cf = incident_faces(m_vertices[_inf_v], previousFace);
                ++cf;
            } else edgeVisible = false;
        }
        // Ajouter les arêtes visibles avant le triangle split
        cf = incident_faces(m_vertices[_inf_v], face_start);
        previousFace = cf->idx();
        --cf;
        edgeVisible = true;
        while (edgeVisible == true) {
            for(int i = 0; i < 3; ++i) {
                if (cf->vertices()[(i + 2) % 3] != _inf_v && cf->vertices()[i] != _inf_v) {
                    a = m_vertices[cf->vertices()[(i + 2) % 3]].point();
                    b = m_vertices[cf->vertices()[i]].point();
                }
            }
            if (testOrientation(a, b, p) > 0.0) { // Arête visible depuis p
                // Flip l'arete
                flipEdge(previousFace, cf->idx());
                previousFace = cf->idx();
                cf = incident_faces(m_vertices[_inf_v], previousFace);

                --cf;
            } else edgeVisible = false;
        }
        return;
    }

    // Le point est dans l'enveloppe convexe, trouver dans quelle face elle se
    // trouve

    // Marche de visibilité
    // On part d'une face aléatoire
    int faceIdx = rand() % faceTab.size();
    while (isFaceFictive(faceIdx)) faceIdx = (faceIdx + 1) % faceTab.size();
    int prevFaceIdx = faceIdx;
    do {
        prevFaceIdx = faceIdx;
        faceIdx = takeStepVisibilityMarch(faceIdx, m_vertices.size() - 1);
    } while (faceIdx != -1); // On est arrivé dans le bon triangle
    splitTriangle(m_vertices.size() - 1, prevFaceIdx);
    rearrangeDelaunay(m_vertices.size() - 1);

}


void Mesh2D::rearrangeDelaunay(int vertexIdx) {
    // File contenant des indices de triangles. On retrouve l'arête à tester
    // en prenant l'arête opposée dans le triangle à vertexIdx.
    std::queue<int> file;
    Point a, b, c, d;
    int f1, f2;
    Circulator_on_faces cfbegin = incident_faces(m_vertices[vertexIdx]), cf;
    cf = cfbegin;
    // Ajouter à la file les triangles autour de vertexIdx
    do {
        file.push(cf->idx());
        ++cf;
    } while(cf != cfbegin);

    while (!file.empty()) {
        // On défile
        f1 = file.front();
        file.pop();
        if (isFaceFictive(f1)) continue;
        // Tester si localement de Delaunay : regarder si d dans le cercle du
        // triangle abc (f1)
        a = m_vertices[vertexIdx].point(); // a = nouveau sommet inséré

        for (int i = 0; i < 3; ++i) {
            if (faceTab[f1].vertices()[i] == vertexIdx) {
                // b,c = arête candidate pour le flip
                b = m_vertices[faceTab[f1].vertices()[(i + 1) % 3]].point();
                c = m_vertices[faceTab[f1].vertices()[(i + 2) % 3]].point();
                // f2 = triangle opposé à a.
                f2 = faceTab[f1].adjacentFaces()[i];
                break;
            }
        }
        if (isFaceFictive(f2)) continue;

        // d = sommet opposé au triangle f1 dans f2.
        for (int i = 0; i < 3; ++i) {
            if (faceTab[f2].adjacentFaces()[i] == f1) {
                d = m_vertices[faceTab[f2].vertices()[i]].point();
                break;
            }
        }
        if (etreDansCercle(a, b, c, d)) { // Triangle pas localement de Delaunay
            // -> flip
            flipEdge(f1, f2);
            // Ajouter les deux nouvelles arêtes formées
            file.push(f1);
            file.push(f2);
        }
    }
}
