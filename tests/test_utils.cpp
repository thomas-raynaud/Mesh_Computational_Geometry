#include <catch2/catch.hpp>

#include "utils/math.h"


TEST_CASE( "Find the center of 3 points", "[utils]" ) {
    glm::vec3 a(1, 5, 0);
    glm::vec3 b(3, 3, 0);
    glm::vec3 c(3, 5, 0);
    glm::vec3 o = compute_circumcenter(a, b, c);
    REQUIRE((
        o.x == Approx(2.0) &&
        o.y == Approx(4.0) &&
        o.z == Approx(0.0)
    ));
}