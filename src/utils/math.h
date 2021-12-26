#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>


// Return a, with a.z = a.x^2 + a.y^2
glm::vec3 phi(const glm::vec3 &a);
glm::vec3 compute_center(
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