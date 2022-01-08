#ifndef PREDICATE_H
#define PREDICATE_H

#include <glm/glm.hpp>


// Returns a value > 0 if abc is oriented in the trigonometric direction
float test_orientation(
    const glm::vec3 &a,
    const glm::vec3 &b,
    const glm::vec3 &c
);

// Check if point d is in the triangle abc
bool is_in_triangle(
    const glm::vec3 &a,
    const glm::vec3 &b,
    const glm::vec3 &c,
    const glm::vec3 &d
);
// Check if d is in the circumscribed circle of the triangle abc,
// with abc oriented in the trigonometric direction
bool is_in_circle(
    const glm::vec3 &a,
    const glm::vec3 &b,
    const glm::vec3 &c,
    const glm::vec3 &d
);

#endif  // PREDICATE_H
