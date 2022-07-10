#include "math.h"

#include <algorithm>


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
    const glm::vec3 &a,
    const glm::vec3 &b,
    glm::mat4 &rotation_matrix
) {
    float theta = acos(std::min(1.f, glm::dot(a, b) / (a.length() * b.length())));
    glm::vec3 u = glm::normalize(glm::cross(a, b));

    float cos_t = cos(theta);
    float sin_t = sin(theta);
    glm::vec3 u2(pow(u.x, 2.f), pow(u.y, 2.f), pow(u.z, 2.f));
    rotation_matrix = glm::mat4(
        u2.x + (1 - u2.x) * cos_t,
        u.x * u.y * (1 - cos_t) - u.z * sin_t,
        u.x * u.z * (1 - cos_t) + u.y * sin_t,
        0,
        u.y * u.x * (1 - cos_t) + u.z * sin_t,
        u2.y + (1 - u2.y) * cos_t,
        u.y * u.z * (1 - cos_t) - u.x * sin_t,
        0,
        u.z * u.x * (1 - cos_t) - u.y * sin_t,
        u.z * u.y * (1 - cos_t) + u.x * sin_t,
        u2.z + (1 - u2.z) * cos_t,
        0,
        0, 0, 0, 1
    );
}

void map_point_to_ndc_coordinates(
    const glm::vec2 &p2,
    const int width,
    const int height,
    glm::vec3 &p3
) {
    p3.x = std::max(std::min(p2.x, (float)width), 0.f);
    p3.y = std::max(std::min(p2.y, (float)height), 0.f);
    p3.x = ((p3.x * 2.f) / width) - 1.f;
    p3.y = ((p3.y * 2.f) / height) - 1.f;
    float x2 = pow(p3.x, 2.f);
    float y2 = pow(p3.y, 2.f);
    if (x2 + y2 <= 1.f) {
        p3.z = sqrt(1.f - x2 - y2);
    }
    else {
        p3.z = 0;
    }
}