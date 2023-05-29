#include <catch2/catch.hpp>

#include <sstream>

#include "utils/math.h"
#include "utils/read_3d_file.h"
#include "utils/read_numbers.h"


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

TEST_CASE ( "Read a number from a string", "[utils]") {
    std::string str;
    int i_number;
    float f_number;
    double d_number;
    str = "str";
    REQUIRE(string_to_number(str, i_number) != 0);
    str = "751.25";
    REQUIRE(string_to_number(str, i_number) == 0);
    REQUIRE(string_to_number(str, f_number) == 0);
    REQUIRE(string_to_number(str, d_number) == 0);
    REQUIRE(i_number == 751);
    REQUIRE(f_number == 751.25f);
    REQUIRE(d_number == 751.25);
    str = "751,25856315";
    REQUIRE(string_to_number(str, f_number) == 0);
    REQUIRE(f_number == Approx(751.2585));
    str = "-2.7e-004";
    REQUIRE(string_to_number(str, f_number) == 0);
    REQUIRE(f_number == Approx(-0.00027f).epsilon(0.00001));
    str = "-2.7e004";
    REQUIRE(string_to_number(str, f_number) == 0);
    REQUIRE(f_number == Approx(-27000.0f));
}

TEST_CASE( "Read a OBJ file", "[utils]" ) {
    Data3D obj_data1, obj_data2;
    std::ostringstream oss;
    std::istringstream iss;

    // Wrong syntax -> ERROR
    oss << "wrong syntax" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_THROWS_AS([&](std::istringstream iss, Data3D obj_data1){
        std::cout << "hello" << obj_data1.faces.size() << std::endl;
        read_obj((std::istream&)iss, &obj_data1);
    }, Parse3DFileException);
    oss.str("");

    // Undefined vertices -> ERROR
    oss << "f 1 2 3" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_THROWS_AS([&](){
        read_obj((std::istream&)iss, &obj_data1);
    }, Parse3DFileException);
    oss.str("");

    // Index of face < 1 -> ERROR
    oss << "v 0.1 0.2 0" << std::endl;
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "v 0.4  0    0.3" << std::endl;
    oss << "f 0 1 2" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_THROWS_AS([&](){
        read_obj((std::istream&)iss, &obj_data1);
    }, Parse3DFileException);
    oss.str("");

    // Face with less than 3 vertices -> ERROR
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "v 0.4 0 0.3" << std::endl;
    oss << "f 1 2" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_THROWS_AS([&](){
        read_obj((std::istream&)iss, &obj_data1);
    }, Parse3DFileException);
    oss.str("");

    // Polyline with less than 2 vertices -> ERROR
    oss << "v 0 0.2 0.3 0" << std::endl;
    oss << "l 1" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_THROWS_AS([&](){
        read_obj((std::istream&)iss, &obj_data1);
    }, Parse3DFileException);
    oss.str("");

    // Correct OBJ
    oss << "v 0.152468428 -1.83673 0" << std::endl;
    oss << "# comment" << std::endl;
    oss << " " << std::endl;
    oss << std::endl;
    oss << "v 0 -0.2 -0.3 0" << std::endl;
    oss << "v 0.4 0 0.3" << std::endl;
    oss << "f 1 2 3" << std::endl;
    oss << "l 1 2 3" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_NOTHROW([&](){
        read_obj((std::istream&)iss, &obj_data2);
    });
    REQUIRE(obj_data2.vertices.size() == 3);
    REQUIRE(obj_data2.vertices[0][0] == 0.152468428f);
    REQUIRE(obj_data2.vertices[0][1] == -1.83673f);
    REQUIRE(obj_data2.vertices[0][2] == 0);
    REQUIRE(obj_data2.vertices[1][0] == 0);
    REQUIRE(obj_data2.vertices[1][1] == -0.2f);
    REQUIRE(obj_data2.vertices[1][2] == -0.3f);
    REQUIRE(obj_data2.faces.size() == 1);
    REQUIRE(obj_data2.faces[0][0] == 0);
    REQUIRE(obj_data2.faces[0][1] == 1);
    REQUIRE(obj_data2.faces[0][2] == 2);
    REQUIRE(obj_data2.polylines.size() == 1);
    REQUIRE(obj_data2.polylines[0].size() == 3);
    REQUIRE(obj_data2.polylines[0][0] == 1);
    REQUIRE(obj_data2.polylines[0][1] == 2);
    REQUIRE(obj_data2.polylines[0][2] == 3);
}

TEST_CASE( "Read a OFF file", "[utils]" ) {
    Data3D off_data;
    std::ostringstream oss;
    std::istringstream iss;

    // Wrong syntax -> ERROR
    oss << "wrong syntax" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_THROWS_AS([&](){
        read_off((std::istream&)iss, &off_data);
    }, Parse3DFileException);
    oss.str("");

    // Correct OFF
    oss << "5 3 0" << std::endl;
    oss << "-0.06939 0.04081 0.14433" << std::endl;
    oss << "1 1 1" << std::endl;
    oss << "1 1 3" << std::endl;
    oss << "1 1 4" << std::endl;
    oss << "1 1 5" << std::endl;
    oss << "3 0 1 2" << std::endl;
    oss << "3 1 3 4" << std::endl;
    oss << "3 2 1 4" << std::endl;
    oss << "3 0 3 2" << std::endl;
    iss = std::istringstream(oss.str());
    REQUIRE_NOTHROW([&](){
        read_off((std::istream&)iss, &off_data);
    });
    REQUIRE(off_data.vertices.size() == 5);
    REQUIRE(off_data.vertices[0][0] == -0.06939f);
    REQUIRE(off_data.vertices[0][1] == 0.04081f);
    REQUIRE(off_data.vertices[0][2] == 0.14433f);
    REQUIRE(off_data.vertices[0][3] == 0);
    REQUIRE(off_data.faces.size() == 3);
    REQUIRE(off_data.faces[0][0] == 0);
    REQUIRE(off_data.faces[0][1] == 1);
    REQUIRE(off_data.faces[0][2] == 2);
}