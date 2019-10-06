#include "mesh.h"

std::vector<Point> Mesh::getLaplacians() {
    if (laplacianTab.size() != 0) return laplacianTab;
    float A; // Aire
    Point a, b, c, d; // Points adjacents au sommet traité.
    Iterator_on_vertices it_v;
    Circulator_on_faces cf, cfbegin;
    Circulator_on_vertices cv, cvbegin, cvtemp;
    double lap_x, lap_y, lap_z;
    double cot_alpha, cot_beta, angles; // Calculs de trigo

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
            cot_alpha = dotProduct(difference(a,b), difference(c,b))
                    / dotProduct(crossProduct(difference(c,b),difference(a,b)),
                                 Point(0,0,1));

            // cot beta
            cot_beta = dotProduct(difference(a,d), difference(c,d))
                    / dotProduct(crossProduct(difference(c,d),difference(a,d)),
                                 Point(0,0,1));

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
