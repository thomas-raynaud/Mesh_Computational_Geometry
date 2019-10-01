/*
 * Classe pour travailler avec des points ou des vecteurs.
*/

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <cmath>
#include <iostream>

class Point {
protected:
    double _x;
    double _y;
    double _z;

public:
    Point(): _x(0), _y(0), _z(0) {}
    Point(double x, double y, double z):_x(x),_y(y),_z(z) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
    double& operator[] (int x) {
        switch(x) {
            case 0: return _x;
            case 1: return _y;
            case 2: return _z;
          }
        return _x;
    }
    // Opérations sur des points/vectors
    double norm();
};

Point difference(Point, Point);
Point crossProduct(Point, Point);
double dotProduct(Point a, Point);
Point scalarProduct(double, Point);

// Prédicats géométriques
int testOrientation(const Point &a, const Point &b, const Point &c);
int isInTriangle(const Point &a, const Point &b, const Point &c, const Point &d);
//d est dans le cercle circonscrit à (a,b,c) ?
int conflictTriangle(const Point &a, const Point &b, const Point &c, const Point &d);
//On part du tgl (a,b,c), on rajoute le point d, on regarde si l'arret ORRIENTE bc est localement de D
int localementDeDelaunay(const Point &a, const Point &b, const Point &c, const Point &d);

int sign(int val);
//La parabole usuelle
Point phi(const Point &a);

#endif // MATH_UTIL_H
