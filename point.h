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
    Point(float x, float y, float z):_x(x),_y(y),_z(z) {}
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
double findAngle(Point, Point, Point);

#endif // MATH_UTIL_H
