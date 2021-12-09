#include "Vertex.h"


int Vertex::vertex_count = 0;


Vertex::Vertex(
        glm::vec3 pos,
        Face *incident_face
    ) : m_pos(pos), m_incident_face(incident_face) { init(); }

Vertex::Vertex() { init(); }


void Vertex::init() {
    m_ind = vertex_count;
    ++vertex_count;
}


glm::vec3 Vertex::get_position() const{
    return m_pos;
}

glm::vec3 Vertex::get_color() const {
    return m_color;
}

Face* Vertex::get_incident_face() const {
    return m_incident_face;
}

Vertex_Hash Vertex::get_hash() const {
    return m_ind;
}
    
void Vertex::set_position(glm::vec3 pos) {
    m_pos = pos;
}

void Vertex::set_color(glm::vec3 color) {
    m_color = color;
}

void Vertex::set_incident_face(Face *incident_face) {
    m_incident_face = incident_face;
}

bool Vertex::operator==(const Vertex& v) const {
    return m_ind == v.m_ind;
}

bool Vertex::operator!=(const Vertex& v) const {
    return m_ind != v.m_ind;
}