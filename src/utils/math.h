#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>


// Return a, with a.z = a.x^2 + a.y^2
glm::vec3 phi(const glm::vec3 &a);
// Compute the center of the circle circumscribed to the triangle (a, b, c)
glm::vec3 compute_circumcenter(
    const glm::vec3 &a,
    const glm::vec3 &b,
    const glm::vec3 &c
);
float cos(
    const glm::vec3 &a,
    const glm::vec3 &b,
    const glm::vec3 &c
);

#endif  // MATH_H