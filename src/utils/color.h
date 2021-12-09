/*
 * Color processing
*/

#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>


// Color space change: from HSV to RGB
glm::vec3 hsv2rgb(int H, double S, double V);

#endif  // COLOR_H