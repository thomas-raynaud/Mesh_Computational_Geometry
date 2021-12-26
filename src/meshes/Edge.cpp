#include "Edge.h"

#include "Vertex.h"
#include "Face.h"


Edge::Edge(Vertex *va, Vertex *vb) {
    init_edge(va, vb);
}


Edge::Edge(Vertex *va, Vertex *vb, Face *_fe, Vertex *_ve) : fe(_fe), ve(_ve) {
    init_edge(va, vb);
}


void Edge::init_edge(Vertex *va, Vertex *vb) {
    if (va->get_hash() < vb->get_hash()) {
        v1 = va;
        v2 = vb;
    }
    else {
        v1 = vb;
        v2 = va;
    }
}


float Edge::get_length() const {
    return glm::distance(v1->get_position(), v2->get_position());
}


Edge_Hash Edge::get_hash() {
    return std::to_string(v1->get_hash()) + "-" + std::to_string(v2->get_hash());
}


bool Edge::operator<(const Edge& e) const {
    return get_length() < e.get_length();
}