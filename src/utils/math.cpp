#include "math.h"

#include <algorithm>
#include<glm/gtc/quaternion.hpp>


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
    const glm::vec3 ba = a - b;
    const glm::vec3 bc = c - b;
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
    const float theta = acos(
        std::min(   1.f,
                    glm::dot(a, b) / (glm::length(a) * glm::length(b))
        )
    );
    const glm::vec3 u = glm::normalize(glm::cross(a, b));

    const float cos_t = cos(theta);
    const float sin_t = sin(theta);
    glm::vec3 u2(pow(u.x, 2.f), pow(u.y, 2.f), pow(u.z, 2.f));
    rotation_matrix = glm::mat4(
        u2.x + (1.f - u2.x) * cos_t,
        u.x * u.y * (1.f - cos_t) - u.z * sin_t,
        u.x * u.z * (1.f - cos_t) + u.y * sin_t,
        0,
        u.y * u.x * (1.f - cos_t) + u.z * sin_t,
        u2.y + (1 - u2.y) * cos_t,
        u.y * u.z * (1.f - cos_t) - u.x * sin_t,
        0,
        u.z * u.x * (1.f - cos_t) - u.y * sin_t,
        u.z * u.y * (1.f - cos_t) + u.x * sin_t,
        u2.z + (1.f - u2.z) * cos_t,
        0,
        0, 0, 0, 1
    );
}

void compute_quaternion_rotation(
    const glm::vec3 &a,
    const glm::vec3 &b,
    glm::quat &quaternion_rotation
) {
    const float theta = acos(
        std::min(   1.f,
                    glm::dot(a, b) / (glm::length(a) * glm::length(b))
        )
    );
    const glm::vec3 u = glm::normalize(glm::cross(a, b));

    const float sin_theta_2 = sin(theta / 2);
    quaternion_rotation = glm::quat(
        cos(theta / 2),
        u.x * sin_theta_2,
        u.y * sin_theta_2,
        u.z * sin_theta_2
    );
}

void map_point_to_ndc_coordinates(
    glm::vec2 p2,
    const int width,
    const int height,
    glm::vec3 &p3
) {
    p2 = glm::clamp(p2, glm::vec2(0, 0), glm::vec2(width, height));
    p3.x = ((p2.x * 2.f) / width) - 1.f;
    p3.y = ((p2.y * 2.f) / height) - 1.f;
    p3.z = 0.f;
    const float dist = glm::dot(p3, p3);
    if (dist <= 1.f) {
        p3.z = sqrt(1.f - dist);
    }
}

void compute_perspective_matrix(
    glm::mat4 &projection,
    float fov,
    float width, float height,
    float z_near, float z_far
) {
    float f = tan(glm::radians(fov) / 2.f);
    float aspect = width / height;
    float zNear = 0.1f;
    float zFar = 100.f;
    projection = glm::identity<glm::mat4>();
    projection[0][0] = 1.f / (f * aspect);
    projection[1][1] = 1.f / f;
    projection[2][2] = (z_far + z_near) / (z_near - z_far);
    projection[3][2] = (2 * z_far *z_near) / (z_near - z_far);
    projection[2][3] = -1;
    projection[3][3] = 0;
}

float get_random_value(const float min, const float max) {
    return min + (((float) rand()) / (float) RAND_MAX) * (max - min);
}

bool are_points_close(const glm::vec3 a, const glm::vec3 b) {
    return glm::length(a - b) < 0.1;
}