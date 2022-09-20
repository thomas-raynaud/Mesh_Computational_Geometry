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
    const glm::vec3 &a,
    const glm::vec3 &b,
    glm::mat4 &rotation_matrix
);
void compute_quaternion_rotation(
    const glm::vec3 &a,
    const glm::vec3 &b,
    glm::qua<float> &quaternion_rotation
);

// Map a point from screen coordinates to normalized device coordinates
void map_point_to_ndc_coordinates(
    glm::vec2 p2,
    const int width,
    const int height,
    glm::vec3 &p3
);

void compute_perspective_matrix(
    glm::mat4 &projection,
    float fov,
    float width, float height,
    float z_near, float z_far
);

float get_random_value(const float min, const float max);

bool is_point_in_diametral_circle_of_segment(
    const glm::vec3 p,
    const glm::vec3 s1,
    const glm::vec3 s2
);

#endif  // MATH_H