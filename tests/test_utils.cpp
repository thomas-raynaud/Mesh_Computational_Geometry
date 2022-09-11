#include <catch2/catch.hpp>

#include <sstream>

#include "utils/math.h"
#include "utils/read_obj.h"


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

TEST_CASE( "Read a OBJ file", "[utils]" ) {
    ObjData obj_data1, obj_data2;
    std::ostringstream oss;
    std::istringstream iss;
    int res;

    // Wrong syntax -> ERROR
    oss << "wrong syntax" << std::endl;
    iss = std::istringstream(oss.str());
    res = read_obj((std::istream&)iss, &obj_data1);
    REQUIRE(res != 0);
    oss.str("");

    // Undefined vertices -> ERROR
    oss << "f 1 2 3" << std::endl;
    iss = std::istringstream(oss.str());
    res = read_obj((std::istream&)iss, &obj_data1);
    REQUIRE(res != 0);
    oss.str("");

    // Index of face < 1 -> ERROR
    oss << "v 0.1 0.2 0" << std::endl;
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "v 0.4 0 0.3" << std::endl;
    oss << "f 0 1 2" << std::endl;
    iss = std::istringstream(oss.str());
    res = read_obj((std::istream&)iss, &obj_data1);
    REQUIRE(res != 0);
    oss.str("");

    // Face with less than 3 vertices -> ERROR
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "v 0.4 0 0.3" << std::endl;
    oss << "f 1 2" << std::endl;
    iss = std::istringstream(oss.str());
    res = read_obj((std::istream&)iss, &obj_data1);
    REQUIRE(res != 0);
    oss.str("");

    // Polyline with less than 2 vertices -> ERROR
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "l 1" << std::endl;
    iss = std::istringstream(oss.str());
    res = read_obj((std::istream&)iss, &obj_data1);
    REQUIRE(res != 0);
    oss.str("");

    // Correct OBJ
    oss << "v 0.1 0.2 0" << std::endl;
    oss << "# comment" << std::endl;
    oss << " " << std::endl;
    oss << std::endl;
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "v 0.4 0 0.3" << std::endl;
    oss << "f 1 2 3" << std::endl;
    oss << "l 1 2 3" << std::endl;
    iss = std::istringstream(oss.str());
    res = read_obj((std::istream&)iss, &obj_data2);
    REQUIRE(res == 0);
    REQUIRE(obj_data2.vertices.size() == 3);
    REQUIRE(obj_data2.vertices[0][0] == 0.1f);
    REQUIRE(obj_data2.vertices[0][1] == 0.2f);
    REQUIRE(obj_data2.vertices[0][2] == 0);
    REQUIRE(obj_data2.vertices[1][0] == 0);
    REQUIRE(obj_data2.vertices[1][1] == 0.2f);
    REQUIRE(obj_data2.vertices[1][2] == 0.3f);
    REQUIRE(obj_data2.faces.size() == 1);
    REQUIRE(obj_data2.faces[0][0] == 1);
    REQUIRE(obj_data2.faces[0][1] == 2);
    REQUIRE(obj_data2.faces[0][2] == 3);
    REQUIRE(obj_data2.polylines.size() == 1);
    REQUIRE(obj_data2.polylines[0].size() == 3);
    REQUIRE(obj_data2.polylines[0][0] == 1);
    REQUIRE(obj_data2.polylines[0][1] == 2);
    REQUIRE(obj_data2.polylines[0][2] == 3);
}