/*
 * Color processing.
*/

#ifndef COLOR_H
#define COLOR_H

#include <array>

// Color space change: from HSV to RGB
std::array<double, 3> hsv2rgb(int H, double S, double V);

#endif  // COLOR_H