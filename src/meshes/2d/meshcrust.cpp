#include "meshcrust.h"

#include <fstream>
#include <sstream>
#include <string>


Crust::Crust(){
    // Insertion des points du fichier
    std::ifstream file("resources/points_courbe.txt");
    if(!file){ return; } // le fichier ne peut pas s'ouvrir
    std::string line;
    double x, y;
    std::istringstream iss;
    while (!file.eof()) {
        std::getline(file, line);
        iss.str(line);
        iss >> x >> y;
        insertion(Point(x, y, 0));
        iss.clear();
    }

    // Voronoi
    this->buildVoronoi();

    this->_firstVoronoiIndex = this->m_vertices.size();

    // Insérer les centres de Voronoi
    for(int vIndex = 0; vIndex < this->vVertices().size(); vIndex++){
        this->insertion(this->vVertices()[vIndex].point());
    }

}
