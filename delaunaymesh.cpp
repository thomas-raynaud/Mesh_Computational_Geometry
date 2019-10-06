#include "mesh.h"

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
    vertexTab[v00].setFace(f1);
    vertexTab[v01].setFace(f2);
    vertexTab[v10].setFace(f2);
    vertexTab[v11].setFace(f1);
    // Changer les faces adjacentes des faces adjacentes (f02 et f12)
    for (int i = 0; i < 3; ++i) if (faceTab[f02].adjacentFaces()[i] == f1) faceTab[f02].setAdjacentFace(f2, i);
    for (int i = 0; i < 3; ++i) if (faceTab[f12].adjacentFaces()[i] == f2) faceTab[f12].setAdjacentFace(f1, i);
}

void Mesh2D::flipRandomEdge() {
    // Take 2 adjacent nonfictive faces and flip the edge between them.
    int randomFace = rand() % faceTab.size();
    int f2;
    bool foundEdge = false;
    Point a, b, c, d; // Points de deux triangle adjacents pour tester leur concavité
    Point ba, bd, ca, cd;
    for(int i = 0; i < faceTab.size(); ++i) {
        randomFace = (randomFace + 1) % faceTab.size();
        if (isFaceFictive(randomFace)) continue;
        for (int j = 0; j < 3; ++j) { // Trouver une face adjacente
            f2 = faceTab[randomFace].adjacentFaces()[j];
            if (isFaceFictive(f2)) continue;
            // Vérifier que les deux triangles ne forment pas un angle concave
            a = vertexTab[faceTab[randomFace].vertices()[j]].point();
            b = vertexTab[faceTab[randomFace].vertices()[(j + 1) % 3]].point();
            c = vertexTab[faceTab[randomFace].vertices()[(j + 2) % 3]].point();
            for (int k = 0; k < 3; ++k) {
                if (faceTab[f2].adjacentFaces()[k] == randomFace) {
                    d = vertexTab[faceTab[f2].vertices()[k]].point();
                    break;
                }
            }
            // Tester les angles ABD et ACD
            ba = difference(b, a);
            bd = difference(b, d);
            ca = difference(c, a);
            cd = difference(c, d);
            if (crossProduct(ba, bd).z() < 0 && crossProduct(cd, ca).z() < 0) {
                foundEdge = true;
                break;
            }
        }
        if (foundEdge == true) break;
    }
    if (foundEdge == false) return;
    flipEdge(randomFace, f2);
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
    this->vertexTab[verticesOfFace[0]].setFace(faceAIndex);
    this->vertexTab[verticesOfFace[1]].setFace(faceBIndex);
    this->vertexTab[verticesOfFace[2]].setFace(faceIndex);
    this->vertexTab[vertexIndex].setFace(faceAIndex);
}

void Mesh2D::splitRandomTriangle() {
    // Take 2 adjacent nonfictive faces and flip the edge between them.
    int randomFace = rand() % faceTab.size();
    // Générer le point au milieu de la face
    std::array<int, 3> vs = faceTab[randomFace].vertices();
    Point a = vertexTab[vs[0]].point();
    Point b = vertexTab[vs[1]].point();
    Point c = vertexTab[vs[2]].point();
    Point p((a.x() + b.x() + c.x()) / 3, (a.y() + b.y() + c.y()) / 3, a.z());
    vertexTab.push_back(Vertex(p, -1, vertexTab.size())); // Ajouter le point en tant que sommet
    splitTriangle(vertexTab.size() - 1, randomFace);
}

void Mesh2D::insertion(Point p) {
    // Ajouter le point p dans le sac de sommets
    vertexTab.push_back(Vertex(p, -1, vertexTab.size()));
    // Regarder si p est à l'extérieur de l'enveloppe convexe
    Point a, b, c;
    int splitFace = -1, new_vertex = vertexTab.size() - 1, previousFace, face_start;
    bool edgeVisible, foundFace;
    Circulator_on_faces cf, cfbegin;
    cfbegin = this->incident_faces(vertexTab[_inf_v]); // Circulateur des faces du point infini
    cf = cfbegin;
    do {
        for(int i = 0; i < 3; ++i) {
            if (cf->vertices()[(i + 2) % 3] != _inf_v && cf->vertices()[i] != _inf_v) {
                a = vertexTab[cf->vertices()[(i + 2) % 3]].point();
                b = vertexTab[cf->vertices()[i]].point();
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
        cfbegin = incident_faces(vertexTab[_inf_v]);
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
                    a = vertexTab[cf->vertices()[(i + 2) % 3]].point();
                    b = vertexTab[cf->vertices()[i]].point();
                }
            }
            if (testOrientation(a, b, p) > 0.0) { // Arête visible depuis p
                // Flip l'arete
                flipEdge(cf->idx(), previousFace);
                cf = incident_faces(vertexTab[_inf_v], previousFace);
                ++cf;
            } else edgeVisible = false;
        }
        // Ajouter les arêtes visibles avant le triangle split
        cf = incident_faces(vertexTab[_inf_v], face_start);
        previousFace = cf->idx();
        --cf;
        edgeVisible = true;
        while (edgeVisible == true) {
            for(int i = 0; i < 3; ++i) {
                if (cf->vertices()[(i + 2) % 3] != _inf_v && cf->vertices()[i] != _inf_v) {
                    a = vertexTab[cf->vertices()[(i + 2) % 3]].point();
                    b = vertexTab[cf->vertices()[i]].point();
                }
            }
            if (testOrientation(a, b, p) > 0.0) { // Arête visible depuis p
                // Flip l'arete
                flipEdge(previousFace, cf->idx());
                previousFace = cf->idx();
                cf = incident_faces(vertexTab[_inf_v], previousFace);

                --cf;
            } else edgeVisible = false;
        }
        return;
    }

    // Le point est dans l'enveloppe convexe, trouver dans quelle face elle se
    // trouve
    for (QVector<Face>::iterator face_it = faceTab.begin(); face_it != faceTab.end(); ++face_it) {
        a = vertexTab[face_it->vertices()[0]].point();
        b = vertexTab[face_it->vertices()[1]].point();
        c = vertexTab[face_it->vertices()[2]].point();
        if (isInTriangle(a, b, c, p) && !isFaceFictive(face_it->idx())) {
            splitTriangle(vertexTab.size() - 1, face_it->idx());
            rearrangeDelaunay(vertexTab.size() - 1);
            return;
        }
    }
    vertexTab.pop_back(); // ERROR, TODO: check
}

void Mesh2D::rearrangeDelaunay(int vertexIdx) {
    // File contenant des indices de triangles. On retrouve l'arête à tester
    // en prenant l'arête opposée dans le triangle à vertexIdx.
    std::queue<int> file;
    Point a, b, c, d;
    int f1, f2;
    Circulator_on_faces cfbegin = incident_faces(vertexTab[vertexIdx]), cf;
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
        a = vertexTab[vertexIdx].point(); // a = nouveau sommet inséré

        for (int i = 0; i < 3; ++i) {
            if (faceTab[f1].vertices()[i] == vertexIdx) {
                // b,c = arête candidate pour le flip
                b = vertexTab[faceTab[f1].vertices()[(i + 1) % 3]].point();
                c = vertexTab[faceTab[f1].vertices()[(i + 2) % 3]].point();
                // f2 = triangle opposé à a.
                f2 = faceTab[f1].adjacentFaces()[i];
                break;
            }
        }
        if (isFaceFictive(f2)) continue;

        // d = sommet opposé au triangle f1 dans f2.
        for (int i = 0; i < 3; ++i) {
            if (faceTab[f2].adjacentFaces()[i] == f1) {
                d = vertexTab[faceTab[f2].vertices()[i]].point();
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
