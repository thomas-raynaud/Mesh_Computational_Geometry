/*
 * Classe pour travailler avec des points ou des vecteurs.
*/

#ifndef POINT_H
#define POINT_H

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
    Point operator* (int v) {
        return Point(_x * v, _y * v, _z * v);
    }
    Point operator+ (Point p) {
        return Point(_x + p._x, _y + p._y, _z + p._z);
    }
    // Opérations sur des points/vectors
    double norm();

    friend Point operator*(const double& v, const Point& p);
    friend Point operator*(const Point& p, const double& v);
    friend Point operator+(const Point& p1, const Point& p2);
};

Point difference(Point, Point);
Point crossProduct(Point, Point);
double dotProduct(Point, Point);
double tangente(Point, Point, Point);
Point operator*(const double& v, const Point& p);
Point operator+(const Point& p1, const Point& p2);

double sign(double v);


//La parabole x*x + y*y
Point phi(const Point &a);

#endif // POINT_H
