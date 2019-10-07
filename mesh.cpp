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
            cot_alpha = dotProduct(difference(a,b), difference(c,b)) / dotProduct(crossProduct(difference(c,b),difference(a,b)), Point(0,0,1));

            // cot beta
            cot_beta = dotProduct(difference(a,d), difference(c,d)) / dotProduct(crossProduct(difference(c,d),difference(a,d)), Point(0,0,1));

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
        if (faceTab[randomFace].isFictive()) {
            randomFace = (randomFace + 1) % faceTab.size();
            continue;
        }
        for (int j = 0; j < 3; ++j) { // Trouver une face adjacente
            f2 = faceTab[randomFace].adjacentFaces()[j];
            if (faceTab[f2].isFictive()) continue;
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
        randomFace = (randomFace + 1) % faceTab.size();
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
    Face faceA = Face(vertexTmp, facesTmp);
    this->faceTab.push_back(faceA);

    //Set faceB
    vertexTmp = {vertexIndex, verticesOfFace[1], verticesOfFace[2]};
    facesTmp = {adjacentFaces[0], faceIndex, faceAIndex};
    Face faceB = Face(vertexTmp, facesTmp);
    this->faceTab.push_back(faceB);

    //Modification de la face splité
    this->faceTab[faceIndex].setVertices(std::array<int,3>{vertexIndex, verticesOfFace[2], verticesOfFace[0]});
    this->faceTab[faceIndex].setAdjacentFaces(std::array<int,3>{adjacentFaces[1], faceAIndex, faceBIndex});

    //Modfication de la face incidente à A
    //Recuperation de l'indice local du sommet opposé à la face A dans la face incidente à A
    vertexTmp = this->faceTab[adjacentFaces[2]].vertices();
    facesTmp = this->faceTab[adjacentFaces[2]].adjacentFaces();
    for(int i = 0; i<3; i++){
        if((vertexTmp[i] != verticesOfFace[0]) && (vertexTmp[i] != verticesOfFace[1])){
            facesTmp[i] = faceAIndex;
            break;
        }
    }
    this->faceTab[adjacentFaces[2]].setAdjacentFaces(facesTmp);

    //Modification de la face incidente à B
    //Recuperation de l'indice local du sommet opposé à la face B dans la face incidente à B
    vertexTmp = this->faceTab[adjacentFaces[0]].vertices();
    facesTmp = this->faceTab[adjacentFaces[0]].adjacentFaces();
    for(int i = 0; i<3; i++){
        if(vertexTmp[i] != verticesOfFace[1] && vertexTmp[i] != verticesOfFace[2]){
            facesTmp[i] = faceBIndex;
            break;
        }
    }
    this->faceTab[adjacentFaces[0]].setAdjacentFaces(facesTmp);

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
    int count = 0;
    while (faceTab[randomFace].isFictive() && count < faceTab.size()) {
        randomFace = (randomFace + 1) % faceTab.size();
        count++;
    }
    if (count == faceTab.size()) return; // pas de face non fictive
    // Générer le point au milieu de la face
    std::array<int, 3> vs = faceTab[randomFace].vertices();
    Point a = vertexTab[vs[0]].point();
    Point b = vertexTab[vs[1]].point();
    Point c = vertexTab[vs[2]].point();
    Point p((a.x() + b.x() + c.x()) / 3, (a.y() + b.y() + c.y()) / 3, a.z());
    vertexTab.push_back(Vertex(p, -1, vertexTab.size())); // Ajouter le point en tant que sommet
    splitTriangle(vertexTab.size() - 1, randomFace);
}

int Mesh2D::localementDeDelaunay(int vertexIndex,Face face){
    //le vertex est toujours "a" dans localement de D
    //Trouver b et c dans les figure de zone de conflit
    std::array<int, 2> bEtc;
    int bEtcIndex = 0;
    int aLocalIndex;
    for(int localVertexIndex = 0; localVertexIndex < 3; localVertexIndex++){
        if(face.vertices()[localVertexIndex] != this->vertexTab[vertexIndex].idx()){
            bEtc[bEtcIndex] = face.vertices()[localVertexIndex];
            bEtcIndex++;
        }else{
            aLocalIndex = localVertexIndex;
        }
    }
    //trouver d dans les figures de zone de conflit
    int dIndex;
    int FaceBIndex = face.adjacentFaces()[aLocalIndex];
    for(int localVertexIndex = 0; localVertexIndex < 3; localVertexIndex++){
        int tmpVertexIndex = this->faceTab[FaceBIndex].vertices()[localVertexIndex];
        std::cout<<tmpVertexIndex<<std::endl;
        if((tmpVertexIndex != bEtc[0]) && (tmpVertexIndex != bEtc[1])){
            dIndex = this->faceTab[FaceBIndex].vertices()[localVertexIndex];
        }
    }

    //tester si l'arrête est localement de D
    Point aLoc = this->vertexTab[vertexIndex].point();
    Point bLoc = this->vertexTab[bEtc[0]].point();
    Point cLoc = this->vertexTab[bEtc[1]].point();
    if(dIndex == -1){
        return 1;
    }
    Point dLoc = this->vertexTab[dIndex].point();//------ERREUR

    return localementDeDelaunayUtil(aLoc, bLoc, cLoc, dLoc);

}

void Mesh2D::defile(std::queue<std::array<int, 2>> *file){
    while(file->size() > 0){
        std::array<int, 2> tmpEdge = file->front();
        file->pop();
        if(localementDeDelaunay(tmpEdge[0], this->faceTab[tmpEdge[1]]) < 0){
            int localVertexIndex;
            for(int localVertexIndexTmp = 0; localVertexIndexTmp < 3; localVertexIndexTmp++){
                if(this->faceTab[tmpEdge[1]].vertices()[localVertexIndexTmp] == tmpEdge[0]){
                    localVertexIndex=localVertexIndexTmp;
                }
            }
            flipEdge(tmpEdge[1], this->faceTab[tmpEdge[1]].adjacentFaces()[localVertexIndex]);
        }
    }
}

void Mesh2D::insertion(Point p){

    bool into = false;
    int faceIndex = 0;
    while(!into && (faceIndex < this->faceTab.size())){
        //Projection du point sur le plan du triangle et changement de base
        //Calcul des vecteurs directeurs du plan du triangle
        std::array<int, 3> vertexOfface;
        if (this->faceTab[faceIndex].isFictive()) {
            faceIndex++;
            continue;
        }
        vertexOfface = this->faceTab[faceIndex].vertices();
       /* Point e1 = difference(this->vertexTab[vertexOfface[1]].point(), this->vertexTab[vertexOfface[0]].point());
        Point e2 = difference(this->vertexTab[vertexOfface[2]].point(), this->vertexTab[vertexOfface[0]].point());

        //Calcul des coordoné du projeté de p sur le plan du triangle dans la base e1, e2
        double p1p = dotProduct(p, e1)/(e1.norm()*e1.norm());
        double p2p = dotProduct(p, e2)/(e2.norm()*e2.norm());
*/
        Point a = vertexTab[vertexOfface[0]].point();
        Point b = vertexTab[vertexOfface[1]].point();
        Point c = vertexTab[vertexOfface[2]].point();

        if(isInTriangle(a, b, c, p) > 0){
            int vertexIndex = this->vertexTab.size();
            this->vertexTab.push_back(Vertex(p, 0, vertexIndex));
            splitTriangle(vertexIndex, faceIndex);
            into=true;

            //Une fois que le point est inseré par split, il faut faire les flips (TP3)
            std::queue<std::array<int, 2>> file;
            bool ZoneDeConflitVide = false;

            while(!ZoneDeConflitVide){
                //On enfile la zone de conflit
                Circulator_on_faces cf, cfbegin;
                cfbegin = this->incident_faces(this->vertexTab[vertexIndex]);
                cf = cfbegin;
                do{
                    int localementDeD = localementDeDelaunay(vertexIndex, *cf); //------ERREUR
                    if(localementDeD < 0){
                        file.push(std::array<int,2> {cf->idx(), vertexIndex});
                    }
                    cf++;
                }while(cf != cfbegin);

                //On defile si necessaire
                if(file.size() > 0){
                    defile(&file);
                }else{
                    ZoneDeConflitVide = true;
                }
            }
        }
        faceIndex++;
    }
}

void Mesh::print(){
   int faceIndex = 0;
   while(faceIndex <= faceTab.size()-1){
       std::cout<<"face ";
       std::cout<<faceIndex<<std::endl;
       std::cout<<"-------"<<std::endl;
       for(int i = 0; i<=2; i++){
          std::cout<<faceTab[faceIndex].vertices()[i];
          std::cout<<"  ";

       }
       std::cout<<std::endl;
       for(int i = 0; i<=2 ; i++){
        std::cout<<faceTab[faceIndex].adjacentFaces()[i];
        std::cout<<"  ";
       }
       std::cout<<std::endl;
       std::cout<<"--------";
       std::cout<<std::endl;
       std::cout<<std::endl;

       faceIndex++;
   }
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
                faceTab[i].setAdjacentFace(faceTab.size() -1, j);
            }
        }
    }

    // Ajout des connexions des faces adjacentes entre faces fictives
    connectAdjacentFaces();
}
Parabola::Parabola(){




    std::cout << "..." << std::endl;


    //Initialisation du maillage, on crée une grosse boite


    // Création des points
/*
    vertexTab.push_back(Vertex(Point(-1,0,0), 0, 0));
    vertexTab.push_back(Vertex(Point(1,-2,0), 0, 1));
    vertexTab.push_back(Vertex(Point(1,2,0), 0, 2));
    vertexTab.push_back(Vertex(Point(0,0,-1), 1, 3));
*/
    vertexTab.push_back(Vertex(Point(-1,0,0), 0, 0));
    vertexTab.push_back(Vertex(Point(1,-1,0), 0, 1));
    vertexTab.push_back(Vertex(Point(1,1,0), 0, 2));
    vertexTab.push_back(Vertex(Point(0,0,-1), 1, 3));


    // Création des faces
    std::array<int, 3> ind_vertices; // indice des sommets
    std::array<int, 3> ind_faces; // indice des faces
    ind_vertices = {0, 1, 2}; ind_faces = {2, 3, 1};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 0
    ind_vertices = {0, 3, 1}; ind_faces = {2, 0, 3};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 1
    ind_vertices = {2, 1, 3}; ind_faces = {1, 3, 0};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 2
    ind_vertices = {0, 2, 3}; ind_faces = {2, 1, 0};
    faceTab.push_back(Face(ind_vertices, ind_faces)); // face 3



/*
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

*/

    //On simule la fonction
    double n=10; //precision du maillage
    for(int xIndex = 0; xIndex < n; xIndex++){
        for(int yIndex = 0; yIndex < n; yIndex++){
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
        }
    }
    // insertion(Point(0, 0, 0));

     print();
}

Mesh2D::Mesh2D() {
    // Création des points
    vertexTab.push_back(Vertex(Point(-0.5,-0.5,0), 0, 0));
    vertexTab.push_back(Vertex(Point(0.5,-0.5,0), 0, 1));
    vertexTab.push_back(Vertex(Point(-0.5,0.5,0), 0, 2));
    vertexTab.push_back(Vertex(Point(0.5,0.5,0), 1, 3));
    // Création des faces
    faceTab.push_back(Face({0, 1, 2}, {1, 2, 3}));  // face 0
    faceTab.push_back(Face({1, 3, 2}, {5, 0, 4}));  // face 1
    faceTab.push_back(Face({0, 2, -1}, {5, 3, 0})); // face 2
    faceTab.push_back(Face({0, -1, 1}, {4, 0, 2})); // face 3
    faceTab.push_back(Face({1, -1, 3}, {5, 1, 3})); // face 4
    faceTab.push_back(Face({2, 3, -1}, {4, 2, 1})); // face 5
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
