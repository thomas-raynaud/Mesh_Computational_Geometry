/*
 * Traitements sur les couleurs.
*/

#ifndef COLOR_H
#define COLOR_H

#include<cmath>
#include <array>
#include <iostream>

// Changement d'espace de couleurs : de HSV à RGB
std::array<double, 3> hsv2rgb(int h, double s, double v);

#endif // COLOR_H
