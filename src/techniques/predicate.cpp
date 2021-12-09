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
    glm::vec3 pq(p - q);
    glm::vec3 pr(p - r);
    return glm::dot(glm::cross(pq, pr), glm::vec3(0, 0, 1));
}


int is_in_triangle(
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c,
        const glm::vec3 &d
    ) {
    float orientation_abc = test_orientation(a, b, c);
    float orientation_abd = test_orientation(a, b, d);
    float orientation_cad = test_orientation(c, a, d);
    float orientation_cdb = test_orientation(c, d, b);
    if ((orientation_abc > 0.f) != (orientation_abd > 0.f) ||
        (orientation_abc > 0.f) != (orientation_cad > 0.f) ||
        (orientation_abc > 0.f) != (orientation_cdb > 0.f)) {
        return 0; // Different orientation -> d outside
    }
    return 1; // d in the triangle
}


int is_in_circle(
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c,
        const glm::vec3 &d
    ) {
    // p, q, r, s correspond to the points a, b, c, d with p in the middle of
    // the phi parabol
    glm::vec3 p = glm::vec3(0, 0, 0);
    glm::vec3 q = b - a;
    glm::vec3 r = c - a;
    glm::vec3 s = d - a;
    // -sign(((PHI(q) - PHI(p)) X (PHI(r) - PHI(p))) . (PHI(s) - PHI(p)))
    float res = -(glm::dot(glm::cross(phi(q) - phi(p), phi(r) - phi(p)), phi(s) - phi(p)));
    return res > 0;
}
