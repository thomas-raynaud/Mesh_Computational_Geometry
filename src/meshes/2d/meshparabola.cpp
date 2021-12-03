#include "meshparabola.h"

#include <cmath>


Parabola::Parabola(int type) : Mesh2D(), _parabola_type(type) {
    // Changement de profondeur des sommets existants
    for (QVector<Vertex>::iterator vertex_it = m_vertices.begin(); vertex_it != m_vertices.end(); ++vertex_it) {
        if (vertex_it->isFictive()) continue;
        vertex_it->setPoint(phi(vertex_it->point()));
        vertex_it->setDisplay(false);
    }

    double x, y, z;

    //On simule la fonction
    double n = 2000; //precision du maillage
    for(int i = 0; i < n; i++){
        x = -1.0 + (((float) rand()) / (float) RAND_MAX) * 2.0;
        y = -1.0 + (((float) rand()) / (float) RAND_MAX) * 2.0;

        if (_parabola_type == 0)
            z = pow(x, 2) + pow(y, 2);
        else {
            double a = 1;
            double b = 100;
            z = pow(a - x, 2) + b * pow(y - x * x, 2);
            z = z / 150.0;
        }
        insertion(Point(x, y, z));
    }
    // Attribution des couleurs
    // trouver zmin et zmax
    double zmin = DBL_MAX, zmax = 0.0, hue;
    for (QVector<Vertex>::iterator vertex_it = m_vertices.begin(); vertex_it != m_vertices.end(); ++vertex_it) {
        if (vertex_it->isFictive() || !vertex_it->isVisible()) continue;
        zmin = std::min(zmin, vertex_it->point().z());
        zmax = std::max(zmax, vertex_it->point().z());
    }
    for (QVector<Vertex>::iterator vertex_it = m_vertices.begin(); vertex_it != m_vertices.end(); ++vertex_it) {
        // Courbure faible : vert, à courbure forte : rouge
        hue = ((vertex_it->point().z() - zmin) / zmax) * 270.0 + 90.0;
        vertex_it->setColor(hsv2rgb(hue, 1.0, 1.0));
    }
}
