#include "math.h"


glm::vec3 phi(const glm::vec3 &a) {
    return glm::vec3(a.x, a.y, a.x * a.x + a.y * a.y);
}