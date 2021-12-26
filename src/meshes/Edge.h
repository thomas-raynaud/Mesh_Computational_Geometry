#ifndef EDGE_H
#define EDGE_H

#include <string>


class Vertex;
class Face;

typedef std::string Edge_Hash;

struct Edge {
    Vertex *v1;
    Vertex *v2;
    Face *fe;   // One of the faces containing the edge
    Vertex *ve; // Vertex opposite of the edge in the face fe

    Edge(Vertex *va, Vertex *vb);
    Edge(Vertex *va, Vertex *vb, Face *_fe, Vertex *_ve);
    void init_edge(Vertex *va, Vertex *vb);

    float get_length() const;
    Edge_Hash get_hash();

    bool operator<(const Edge& f) const;
};

#endif  // EDGE_H