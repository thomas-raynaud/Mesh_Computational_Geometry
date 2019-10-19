#include "predicate.h"

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
        (orientationABC > 0.f) != (orientationCDB > 0.f)) {
        return 0; // Orientation différente -> d en dehors
    }
    return 1; // d dans le triangle
}

int etreDansCercle(const Point &a, const Point &b, const Point &c, const Point &d) {
    // p, q, r, s correspondent aux points a, b, c, d avec p au milieu de la parabole phi
    Point p = Point(0, 0, 0);
    Point q = difference(b, a);
    Point r = difference(c, a);
    Point s = difference(d, a);
    // -signe(((PHI(q) - PHI(p)) X (PHI(q) - PHI(p))) . (PHI(s) - PHI(p)))
    double res = -(dotProduct(crossProduct(difference(phi(q), phi(p)),
                                           difference(phi(r), phi(p))),
                              difference(phi(s), phi(p))));
    return res > 0;
}
