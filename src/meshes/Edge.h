#ifndef EDGE_H
#define EDGE_H

#include <string>


class Vertex;
class Face;

typedef std::string Edge_Hash;

struct Edge {
    Face *fe;   // One of the faces containing the edge
    Vertex *ve; // Vertex opposite of the edge in the face fe
    Vertex *v1;
    Vertex *v2;

    Edge(Vertex *v1, Vertex *v2);

    float get_length();
    Edge_Hash get_hash();
};

#endif  // EDGE_H