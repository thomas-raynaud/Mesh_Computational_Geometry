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
// Compute the rotation matrix to go from vector a to vector b
void compute_rotation_matrix(
    const glm::vec2 &a,
    const glm::vec2 &b,
    glm::mat4 &rotation_matrix
);

// Map a point from screen coordinates to normalized device coordinates
void map_point_to_ndc_coordinates(
    const glm::vec2 &p2,
    const int width,
    const int height,
    glm::vec3 &p3
);

#endif  // MATH_H