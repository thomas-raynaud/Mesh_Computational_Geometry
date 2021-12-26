#ifndef FACE_H
#define FACE_H

#include <array>
#include <glm/glm.hpp>


class Vertex;

typedef int Face_Hash;

class Face {

public:
    Face(
        std::array<Vertex*, 3> vertices,
        std::array<Face*, 3> adjacent_faces={nullptr, nullptr, nullptr}
    );
    Face();

    std::array<Vertex*, 3> get_vertices() const;
    std::array<Face*, 3> get_adjacent_faces() const;
    glm::vec3 get_color() const;
    Face_Hash get_hash() const;
    float get_max_angle() const;

    void set_vertices(std::array<Vertex*, 3> vertices);
    void set_adjacent_faces(std::array<Face*, 3> adjacent_faces);
    void set_random_color();

    void set_vertex(int ind, Vertex *vertex);
    void set_adjacent_face(int ind, Face *face);

    bool operator==(const Face& f) const;
    bool operator!=(const Face& f) const;

private:
    std::array<Vertex*, 3> m_vertices;
    // Array of adjacent faces. 1st face = face opposite to the 1st vertex, ...
    std::array<Face*, 3> m_adjacent_faces;
    glm::vec3 m_color;
    int m_ind;

    static int face_count;
};

#endif  // FACE_H