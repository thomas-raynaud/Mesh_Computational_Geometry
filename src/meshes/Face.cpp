#include "Face.h"

#include "Vertex.h"
#include "utils/math.h"


int Face::face_count = 0;

Face::Face(
        std::array<Vertex*, 3> vertices,
        std::array<Face*, 3> adjacent_faces
    ) :     m_vertices(vertices),
            m_adjacent_faces(adjacent_faces)
{
    set_random_color();
    m_ind = face_count;
    ++face_count;
}

Face::Face() {
    set_random_color();
    m_ind = face_count;
    ++face_count;
}

std::array<Vertex*, 3> Face::get_vertices() const {
    return m_vertices;
}

std::array<Face*, 3> Face::get_adjacent_faces() const {
    return m_adjacent_faces;
}

glm::vec3 Face::get_color() const {
    return m_color;
}

Face_Hash Face::get_hash() const {
    return m_ind;
}

float Face::get_max_angle() const {
    glm::vec3 a, b, c;
    a = m_vertices[0]->get_position();
    b = m_vertices[1]->get_position();
    c = m_vertices[2]->get_position();
    return std::max(cos(b, a, c), std::max(cos(a, b, c), cos(b, c, a)));
}


void Face::set_vertices(std::array<Vertex*, 3> vertices) {
    m_vertices = vertices;
}

void Face::set_adjacent_faces(std::array<Face*, 3> adjacent_faces) {
    m_adjacent_faces = adjacent_faces;
}

void Face::set_random_color() {
    m_color = glm::vec3(
        (float) rand() / (RAND_MAX),
        (float) rand() / (RAND_MAX),
        (float) rand() / (RAND_MAX)
    );
}

void Face::set_vertex(int ind, Vertex *vertex) {
    m_vertices[ind] = vertex;
}

void Face::set_adjacent_face(int ind, Face *face) {
    m_adjacent_faces[ind] = face;
}

bool Face::operator==(const Face& f) const {
    return m_ind == f.m_ind;
}

bool Face::operator!=(const Face& f) const {
    return m_ind != f.m_ind;
}