#include "point.h"

double Point::norm() {
    return sqrt(_x * _x + _y * _y + _z * _z);
}

Point difference(const Point a, const Point b) {
    return Point(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
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

int testOrientation(const Point &a, const Point &b, const Point &c) {
    // On travaille en 2D -> Z à 0
    Point p(a.x(). a.y(), 0), q(b.x(), b.y(), 0), r(c.x(), c.y(), 0);
    Point pq(difference(p, q));
    Point pr(difference(p, r));
    return dotProduct(crossProduct(pq, pr), Point(0, 0, 1));
}

int isInTriangle(const Point &a, const Point &b, const Point &c, const Point &d) {
    int orientationABC = sign(testOrientation(a, b, c));
    int orientationABD = sign(testOrientation(a, b, d));
    int orientationCAD = sign(testOrientation(a, c, d));
    int orientationCBD = sign(testOrientation(c, b, d));
    if (orientationABC != orientationABD ||
        orientationABC != orientationCAD ||
        orientationABC != orientationCBD) { // Orientation différente -> d en dehors
        return -1;
    } else if (orientationABD == 0 || orientationCAD == 0 || orientationCBD == 0) { // d sur une des arêtes
        return 0;
    }
    return 1; // d dans le triangle
}

int sign(int val) {
    if (val >= 0) return 1;
    return -1;
}
