#include "predicate.h"

#include "utils/math.h"


float test_orientation(
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c
    ) {
    // We work in 2D -> Z at 0
    glm::vec3 p(a.x, a.y, 0);
    glm::vec3 q(b.x, b.y, 0);
    glm::vec3 r(c.x, c.y, 0);
    glm::vec3 pq(q - p);
    glm::vec3 pr(r - p);
    return glm::dot(glm::cross(pq, pr), glm::vec3(0, 0, 1));
}


bool is_in_triangle(
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c,
        const glm::vec3 &d
    ) {
    bool orientation_abc = test_orientation(a, b, c) > 0.f;
    bool orientation_abd = test_orientation(a, b, d) > 0.f;
    bool orientation_cad = test_orientation(c, a, d) > 0.f;
    bool orientation_cdb = test_orientation(c, d, b) > 0.f;
    if (orientation_abc != orientation_abd ||
        orientation_abc != orientation_cad ||
        orientation_abc != orientation_cdb) {
        return false; // Different orientation -> d outside
    }
    return true; // d in the triangle
}


bool is_in_circle(
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c,
        const glm::vec3 &d
    ) {
    // p, q, r, s correspond to the points a, b, c, d with p in the middle of
    // the phi parabol
    glm::vec3 p = phi(glm::vec3(0, 0, 0));
    glm::vec3 q = phi(b - a);
    glm::vec3 r = phi(c - a);
    glm::vec3 s = phi(d - a);
    // -sign(PQ X PR) . PS)
    float res = -(glm::dot(glm::cross(q - p, r - p), s - p));
    return res > 0;
}

bool is_point_in_diametral_circle_of_segment(
    const glm::vec3 p,
    const glm::vec3 s1,
    const glm::vec3 s2
) {
    glm::vec3 a, b;
    float angle_p;
    float orientation_s1_s2_p = test_orientation(s1, s2, p);
    if (orientation_s1_s2_p > 0.f) {
        a = s1;
        b = s2;
    }
    else if (orientation_s1_s2_p < 0.f) {
        a = s2;
        b = s1;
    }
    else {
        return true;
    }
    angle_p = std::acos(cos(s1, p, s2));
    return glm::degrees(angle_p) >= 90.f;
}