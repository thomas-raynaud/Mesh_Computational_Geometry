
#include "meshcrust.h"


Crust::Crust(){

    // Insertion des points du fichier
    std::ifstream file("points_courbe.txt");
    if(!file){ return; } // le fichier ne peut pas s'ouvrir
    std::string line;
    std::getline(file, line);
    double x, y;
    int delimiterPos1, delimiterPos2;
    while(line != "eof\n"){
        delimiterPos1 = line.find(" ",0);
        x = atof(line.substr(0, delimiterPos1).c_str());
        delimiterPos2 = line.find("\n", delimiterPos1+1);
        y = atof(line.substr(delimiterPos1, delimiterPos2).c_str());

        Point P = Point(x, y, 0);
        this->insertion(P);
    }
    // Voronoi
    this->buildVoronoi();

    this->_firstVoronoiIndex = this->vertexTab.size();
    for(int vIndex = 0; vIndex < this->vVertices().size(); vIndex++){
        this->insertion(this->vVertices()[vIndex].point());
    }

}
