#include "math.h"


glm::vec3 phi(const glm::vec3 &a) {
    return glm::vec3(a.x, a.y, a.x * a.x + a.y * a.y);
}

glm::vec3 compute_circumcenter(
    const glm::vec3 &a,
    const glm::vec3 &b,
    const glm::vec3 &c
) {
    float tan_a, tan_b, tan_c, coeff_a, coeff_b, coeff_c;
    glm::vec3 ac, ab, ba, bc, cb, ca;
    glm::vec3 k(0.f, 0.f, 1.f);
    ac =  c - a;
    ab =  b - a;
    ba =  a - b;
    bc =  c - b;
    cb =  b - c;
    ca =  a - c;

    tan_a = glm::dot(glm::cross(ac, ab), k) * glm::dot(cb, ca) * glm::dot(ba, bc);
    tan_b = glm::dot(glm::cross(ba, bc), k) * glm::dot(cb, ca) * glm::dot(ac, ab);
    tan_c = glm::dot(glm::cross(cb, ca), k) * glm::dot(ab, ac) * glm::dot(ba, bc);

    coeff_a = (tan_c + tan_b);
    coeff_b = (tan_c + tan_a);
    coeff_c = (tan_a + tan_b);

    float sum = 1.f / (coeff_a + coeff_b + coeff_c);

    coeff_a = coeff_a * sum;
    coeff_b = coeff_b * sum;
    coeff_c = coeff_c * sum;

    return coeff_a * a + coeff_b * b + coeff_c * c;
}

float cos(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
    glm::vec3 ba = a - b;
    glm::vec3 bc = c - b;
    if (glm::dot(ba, bc) > glm::length(ba) * glm::length(bc))
        return -1.f;
    else
        return glm::dot(ba, bc) / (glm::length(ba) * glm::length(bc));
}

void compute_rotation_matrix(
    const glm::vec2 &a,
    const glm::vec2 &b,
    glm::mat4 &rotation_matrix
) {
    // TODO
}

void map_point_to_ndc_coordinates(
    const glm::vec2 &p2,
    const int width,
    const int height,
    glm::vec3 &p3
) {
    // TODO
}