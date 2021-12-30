#ifndef MESH_H
#define MESH_H

#include <vector>
#include <unordered_map>

#include "Vertex.h"
#include "Face.h"


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

    // Update the face array by connecting adjacent faces
    void connect_adjacent_faces();

public:
    Mesh();

    int get_nb_vertices();
    int get_nb_faces();

    void add_vertex(Vertex *vtx);
    void add_face(Face *face);

    virtual void pop_vertex(Vertex *v);
    void pop_face(Face *f);

    virtual void draw_mesh_vertices_colors();
    virtual void draw_mesh_faces_colors();
    virtual void draw_mesh_wireframe_vertices_color();
    virtual void draw_mesh_wireframe_faces_color();

    friend class FaceIterator;
    FaceIterator faces_begin();
    FaceIterator faces_end();

    friend class VertexIterator;
    VertexIterator vertices_begin();
    VertexIterator vertices_end();

    friend class FaceCirculator;
    FaceCirculator incident_faces(const Vertex &v);
    // Get circulator starting at face face_start
    FaceCirculator incident_faces(const Vertex &v, const Face &face_start);

    friend class VertexCirculator;
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
private:
    std::vector<Vertex*> m_neighbour_vertices;
    int m_ind;
};

std::ostream& operator<<(std::ostream &strm, const Mesh &mesh);

#endif // MESH_H