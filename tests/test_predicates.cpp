#include <catch2/catch.hpp>

#include "techniques/predicate.h"


TEST_CASE( "Test the orientation of a triangle", "[predicate]" ) {
    glm::vec3 a(0, 0, 0);
    glm::vec3 b(1, 0, 0);
    glm::vec3 c(0, 1, 0);
    REQUIRE(test_orientation(a, b, c) > 0);
    REQUIRE(test_orientation(a, c, b) < 0);
}

TEST_CASE( "Test if a point is in a triangle", "[predicate" ) {
    glm::vec3 a(2.f, 5.f, 0.f);
    glm::vec3 b(3.f, 1.f, 0.f);
    glm::vec3 c(5.f, 3.f, 0.f);
    glm::vec3 d(4.f, 3.f, 0.f);
    REQUIRE(is_in_triangle(a, b, c, d) == true);
    REQUIRE(is_in_triangle(a, b, d, c) == false);
}

TEST_CASE( "Test if points are in the circumscribed circle of a triangle", "[predicate]" ) {
    glm::vec3 a(2.f, 5.f, 0.f);
    glm::vec3 b(3.f, 1.f, 0.f);
    glm::vec3 c(5.f, 3.f, 0.f);
    glm::vec3 d(4.f, 3.f, 0.f);
    glm::vec3 e(2.f, 3.f, 0.f);
    glm::vec3 f(-1.f, 3.f, 0.f);
    REQUIRE(is_in_circle(a, b, c, d) == true);
    REQUIRE(is_in_circle(a, b, c, e) == true);
    REQUIRE(is_in_circle(a, b, c, f) == false);
}

TEST_CASE ( "Is a point in the diametral circle of a segment", "[predicate]" ) {
    glm::vec3 p, s1, s2;
    s1 = glm::vec3(-1, 1, 0);
    s2 = glm::vec3(1, 1, 0);
    p = glm::vec3(0, 0.5, 0);
    REQUIRE(is_point_in_diametral_circle_of_segment(p, s1, s2) == true);
    p = glm::vec3(0, -0.5, 0);
    REQUIRE(is_point_in_diametral_circle_of_segment(p, s1, s2) == false);
    p = s1;
    REQUIRE(is_point_in_diametral_circle_of_segment(p, s1, s2) == true);
    p = glm::vec3(0, 1.5, 0);
    REQUIRE(is_point_in_diametral_circle_of_segment(p, s1, s2) == true);
    p = glm::vec3(0, 2.5, 0);
    REQUIRE(is_point_in_diametral_circle_of_segment(p, s1, s2) == false);
}