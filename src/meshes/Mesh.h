#ifndef MESH_H
#define MESH_H

#include <vector>
#include <unordered_map>
#include <iostream>

#include "Vertex.h"
#include "Face.h"
#include "utils/BoundingBox.h"


class FaceIterator;
class VertexIterator;
class FaceCirculator;
class VertexCirculator;

typedef std::unordered_map<Face_Hash, Face>::iterator FaceIteratorType;
typedef std::unordered_map<Face_Hash, Face>::const_iterator FaceConstIteratorType;
typedef std::unordered_map<Vertex_Hash, Vertex>::iterator VertexIteratorType;
typedef std::unordered_map<Vertex_Hash, Vertex>::const_iterator VertexConstIteratorType;

class Mesh {

protected:
    std::unordered_map<Vertex_Hash, Vertex> m_vertices;
    std::unordered_map<Face_Hash, Face> m_faces;

public:
    Mesh();
    virtual ~Mesh();

    int get_nb_vertices();
    int get_nb_faces();
    virtual BoundingBox get_bounding_box();

    Vertex* add_vertex(glm::vec3 pos);
    virtual Face* add_face(std::array<Vertex*, 3> face_vts);

    virtual void pop_vertex(Vertex *v);
    void pop_face(Face *f);

    // Update the face array by connecting adjacent faces
    void connect_adjacent_faces();

    friend class FaceIterator;
    FaceIterator faces_begin();
    FaceIterator faces_end();

    friend class VertexIterator;
    VertexIterator vertices_begin();
    VertexIterator vertices_end();

    friend class FaceCirculator;
    // Get faces incident to vertex v. If v is close to a boundary
    // (nullptr faces), return an empty face circulator.
    FaceCirculator incident_faces(const Vertex &v);
    // Get circulator starting at face face_start
    FaceCirculator incident_faces(const Vertex &v, const Face &face_start);

    friend class VertexCirculator;
    // Get vertices nearby the vertex v. If v is close to a boundary
    // (nullptr faces), return an empty vertex circulator.
    VertexCirculator neighbour_vertices(const Vertex &v);

    friend std::ostream& operator<<(std::ostream &strm, const Mesh &m);
};


class FaceIterator {
public:
    FaceIterator();
    FaceIterator(FaceIteratorType face_it);
    bool operator!=(const FaceIterator& it) const;
    bool operator==(const FaceIterator& it) const;
    Face& operator*();
    Face* operator->();
    FaceIterator& operator++();
    FaceIterator operator++(int);
private:
    FaceIteratorType m_face_it;
};


class VertexIterator {
public:
    VertexIterator();
    VertexIterator(VertexIteratorType face_it);
    bool operator!=(const VertexIterator& it) const;
    bool operator==(const VertexIterator& it) const;
    Vertex& operator*();
    Vertex* operator->();
    VertexIterator& operator++();
    VertexIterator operator++(int);
private:
    VertexIteratorType m_vertex_it;
};


class FaceCirculator {
public:
    FaceCirculator(std::vector<Face*> incident_faces);
    FaceCirculator();
    bool operator!=(const FaceCirculator& it) const;
    Face& operator*();
    Face* operator->();
    FaceCirculator& operator++();
    FaceCirculator& operator--();
    FaceCirculator operator++(int);
    size_t get_nb_incident_faces();
private:
    std::vector<Face*> m_incident_faces;    // Incident faces to a vertex
    int m_ind;                              // Current index in m_incident_faces
};


class VertexCirculator {
public:
    VertexCirculator(std::vector<Vertex*> neighbor_vertices);
    VertexCirculator();
    bool operator!=(const VertexCirculator& it) const;
    Vertex& operator*();
    Vertex* operator->(); 
    VertexCirculator& operator++();
    VertexCirculator& operator--();
    VertexCirculator operator++(int);
    size_t get_nb_neighbour_vertices();
private:
    std::vector<Vertex*> m_neighbour_vertices;
    int m_ind;
};

std::ostream& operator<<(std::ostream &strm, const Mesh &mesh);

#endif // MESH_H
