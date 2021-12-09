#include "Edge.h"

#include "Vertex.h"
#include "Face.h"


Edge::Edge(Vertex *_v1, Vertex *_v2) : v1(_v1), v2(_v2) {}

float Edge::get_length() {
    return glm::distance(v1->get_position(), v2->get_position());
}

Edge_Hash Edge::get_hash() {
    return std::to_string(v1->get_hash()) + "-" + std::to_string(v2->get_hash());
}