#include "point.h"

double Point::norm() {
    return sqrt(_x * _x + _y * _y + _z * _z);
}

Point difference(const Point a, const Point b) {
    return Point(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}
Point somme(const Point a, const Point b) {
    return Point(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}


Point crossProduct(Point u, Point v) {
    double w1 = u.y() * v.z() - u.z() * v.y();
    double w2 = u.z() * v.x() - u.x() * v.z();
    double w3 = u.x() * v.y() - u.y() * v.x();
    return Point(w1, w2, w3);
}

double dotProduct(Point a, Point b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

Point scalarProduct(double t, Point x){
    return Point(t*x.x(), t*x.y(), t*x.z());
}

float testOrientation(const Point &a, const Point &b, const Point &c) {
    // On travaille en 2D -> Z à 0
    Point p(a.x(), a.y(), 0), q(b.x(), b.y(), 0), r(c.x(), c.y(), 0);
    Point pq(difference(p, q));
    Point pr(difference(p, r));
    return dotProduct(crossProduct(pq, pr), Point(0, 0, 1));
}

int isInTriangle(const Point &a, const Point &b, const Point &c, const Point &d) {
    float orientationABC = testOrientation(a, b, c);
    float orientationABD = testOrientation(a, b, d);
    float orientationCAD = testOrientation(c, a, d);
    float orientationCDB = testOrientation(c, d, b);
    if ((orientationABC > 0.f) != (orientationABD > 0.f) ||
        (orientationABC > 0.f) != (orientationCAD > 0.f) ||
        (orientationABC > 0.f) != (orientationCDB > 0.f)) { // Orientation différente -> d en dehors
        return 0;
    }
    return 1; // d dans le triangle
}

Point phi(const Point &a) {
    return Point(a.x(), a.y(), a.x() * a.x() + a.y() * a.y());
}

int etreDansCercle(const Point &a, const Point &b, const Point &c, const Point &d) {
    // p, q, r, s correspondent aux points a, b, c, d avec p au milieu de la parabole phi
    Point p = Point(0, 0, 0);
    Point q = difference(b, a);
    Point r = difference(c, a);
    Point s = difference(d, a);
    // -signe(((PHI(q) - PHI(p)) X (PHI(q) - PHI(p))) . (PHI(s) - PHI(p)))
    double res = -(dotProduct(crossProduct(difference(phi(q), phi(p)), difference(phi(r), phi(p))), difference(phi(s), phi(p))));
    return res > 0;
}
double tan(Point a, Point b, Point c){
    //Tangente d'un angle ABC
    Point BA;
    Point BC;
    BA = difference(a, b);
    BC = difference(c, b);
    double num = dotProduct(crossProduct(BC, BA), Point(0,0,1));
    double den = dotProduct(BA, BC);

    return num / den;

}
