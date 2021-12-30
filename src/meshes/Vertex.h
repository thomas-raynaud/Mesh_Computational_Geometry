#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

#define MAX_DIST 20.f


class Face;

typedef int Vertex_Hash;

class Vertex {

public:
    Vertex(glm::vec3 pos, Face *incident_face=nullptr);
    Vertex() {}

    glm::vec3 get_position() const;
    glm::vec3 get_color() const;
    Face* get_incident_face() const;
    Vertex_Hash get_hash() const;
    
    void set_position(glm::vec3 pos);
    void set_color(glm::vec3 color);
    void set_incident_face(Face *incident_face);

    bool operator==(const Vertex& v) const;
    bool operator!=(const Vertex& v) const;

private:
    glm::vec3 m_pos;
    glm::vec3 m_color;
    Face *m_incident_face;
    int m_ind;

    static int vertex_count;

    void init();
};

#endif  // VERTEX_H