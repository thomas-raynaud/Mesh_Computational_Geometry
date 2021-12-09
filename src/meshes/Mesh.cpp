#include "Mesh.h"

#include "Edge.h"


Mesh::Mesh() {}

int Mesh::get_nb_vertices() {
    return m_vertices.size();
}


void Mesh::connect_adjacent_faces() {
    std::unordered_map<Edge_Hash, Edge> edges; // Edges of the mesh
    std::array<Vertex*, 3> curr_face_vts;
    std::array<Vertex*, 3> adj_face_vts;
    int v1_hash, v2_hash;
    Face *curr_face;
    std::unordered_map<Face_Hash, Face>::iterator face_it;
    for (face_it = m_faces.begin(); face_it != m_faces.end(); ++face_it) {
        curr_face = &face_it->second;
        curr_face_vts = face_it->second.get_vertices();
        for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
            v1_hash = std::min(
                curr_face_vts[v_ind]->get_hash(),
                curr_face_vts[(v_ind + 1) % 3]->get_hash()
            );
            v2_hash = std::max(
                curr_face_vts[v_ind]->get_hash(),
                curr_face_vts[(v_ind + 1) % 3]->get_hash()
            );
            Edge e(&m_vertices[v1_hash], &m_vertices[v2_hash]);
            e.fe = curr_face;
            if (edges.find(e.get_hash()) == edges.end()) { // New edge
                edges[e.get_hash()] = e;
            } else {
                // The edge has already been registered. The current face is
                // adjacent with the edge's linked face.
                Face *adj_face = edges[e.get_hash()].fe;
                // Link adjacent face on current face
                curr_face->set_adjacent_face((v_ind + 2) % 3, adj_face);
                // Link current face on adjacent face
                adj_face_vts = adj_face->get_vertices();
                for (size_t v_ind2 = 0; v_ind2 < 3; ++v_ind2) {
                    if (    adj_face_vts[v_ind2]->get_hash() != v1_hash &&
                            adj_face_vts[v_ind2]->get_hash() != v2_hash) {
                        adj_face->set_adjacent_face(v_ind2, curr_face);
                        break;
                    }
                }
            }
        }
    }
}


FaceIterator::FaceIterator() : m_face_it(nullptr) {}
FaceIterator::FaceIterator(FaceIteratorType face_it) {
    m_face_it = face_it;
}
bool FaceIterator::operator!=(const FaceIterator& it) const {
    return m_face_it != it.m_face_it;
}
bool FaceIterator::operator==(const FaceIterator& it) const {
    return m_face_it == it.m_face_it;
}
Face& FaceIterator::operator*()  { return m_face_it->second; }
Face* FaceIterator::operator->() { return &m_face_it->second;  }
FaceIterator& FaceIterator::operator++() { ++m_face_it; return *this; }
FaceIterator FaceIterator::operator++(int) {
    FaceIterator temp = *this;
    ++(*this);
    return temp;
}

FaceIterator Mesh::faces_begin() {
    FaceIterator(m_faces.begin());
}
FaceIterator Mesh::faces_end() {
    return FaceIterator(m_faces.end());
}


VertexIterator::VertexIterator() : m_vertex_it(nullptr) {}
VertexIterator::VertexIterator(VertexIteratorType vertex_it) {
    m_vertex_it = vertex_it;
}
bool VertexIterator::operator!=(const VertexIterator& it) const {
    return m_vertex_it != it.m_vertex_it;
}
bool VertexIterator::operator==(const VertexIterator& it) const {
    return m_vertex_it == it.m_vertex_it;
}
Vertex& VertexIterator::operator*()  { return m_vertex_it->second; }
Vertex* VertexIterator::operator->() { return &m_vertex_it->second; }
VertexIterator& VertexIterator::operator++() { ++m_vertex_it; return *this; }
VertexIterator VertexIterator::operator++(int) {
    VertexIterator temp = *this;
    ++(*this);
    return temp;
}

VertexIterator Mesh::vertices_begin() {
    return VertexIterator(m_vertices.begin());
}

VertexIterator Mesh::vertices_end() {
    return VertexIterator(m_vertices.end());
}


FaceCirculator::FaceCirculator(
        std::vector<Face*> incident_faces
    ) : m_incident_faces(incident_faces), m_ind(0) {}
FaceCirculator::FaceCirculator() : m_ind(-1) {}
bool FaceCirculator::operator!=(const FaceCirculator& it) const {
    return m_incident_faces != it.m_incident_faces || m_ind != it.m_ind;
}
Face& FaceCirculator::operator*() { return *(m_incident_faces[m_ind]); }
Face* FaceCirculator::operator->() { return m_incident_faces[m_ind]; }
FaceCirculator& FaceCirculator::operator++() {
    m_ind = (m_ind + 1) % m_incident_faces.size();
    return *this;
}
FaceCirculator& FaceCirculator::operator--() {
    m_ind--;
    if (m_ind < 0) {
        if (m_incident_faces.size() == 0) m_ind = 0;
        else m_ind = m_incident_faces.size() - 1;
    }
    return *this;
}
FaceCirculator FaceCirculator::operator++(int) {
    FaceCirculator temp = *this;
    ++(*this);
    return temp;
}

FaceCirculator Mesh::incident_faces(const Vertex &v) {
    Face *first_face = v.get_incident_face();
    std::vector<Face*> incident_faces;

    Face *current_face = first_face;
    int opposite_vertex_ind;
    std::array<Vertex *, 3> face_vts;
    // Accumulate incident faces while going around v
    do {
        // Find the index of v in the current face.
        // Opposite vertex is the next index.
        face_vts = current_face->get_vertices();
        for (size_t v_ind = 0; v_ind < 3; ++v_ind) {
            if (*face_vts[v_ind] == v) {
                opposite_vertex_ind = (v_ind + 1) % 3;
                break;
            }
        }
        incident_faces.push_back(current_face);
        current_face = current_face->get_adjacent_faces()[opposite_vertex_ind];
    }
    while(current_face != first_face);

    return FaceCirculator(incident_faces);
}

FaceCirculator Mesh::incident_faces(const Vertex &v, const Face &face_start) {
    FaceCirculator fc(incident_faces(v)), fc_begin;
    fc_begin = fc;
    do {
        if (*fc == face_start) return fc;
        ++fc;
    } while(fc != fc_begin);
    return fc;
}


VertexCirculator::VertexCirculator(
        std::vector<Vertex*> neighbor_vertices
    ): m_neighbour_vertices(neighbor_vertices), m_ind(0) {}
VertexCirculator::VertexCirculator() : m_ind(-1) {}
bool VertexCirculator::operator!=(const VertexCirculator& it) const {
    return m_neighbour_vertices != it.m_neighbour_vertices || m_ind != it.m_ind;
}
Vertex& VertexCirculator::operator*() { return *(m_neighbour_vertices[m_ind]); }
Vertex* VertexCirculator::operator->() { return m_neighbour_vertices[m_ind]; }
VertexCirculator& VertexCirculator::operator++() {
    m_ind = (m_ind + 1) % m_neighbour_vertices.size();
    return *this;
}
VertexCirculator& VertexCirculator::operator--() {
    m_ind--;
    if (m_ind < 0) {
        if (m_neighbour_vertices.size() == 0) m_ind = 0;
        else m_ind = m_neighbour_vertices.size() - 1;
    }
    return *this;
}
VertexCirculator VertexCirculator::operator++(int) {
    VertexCirculator temp = *this;
    ++(*this);
    return temp;
}

VertexCirculator Mesh::neighbour_vertices(const Vertex &v) {
    Face *first_face = v.get_incident_face();
    Face *current_face = first_face;
    std::vector<Vertex*> neighbour_vertices;
    int opposite_vertex_ind;
    std::array<Vertex*, 3> face_vts;
    do {
        for (int v_ind = 0; v_ind < 3; ++v_ind) {
            face_vts = current_face->get_vertices();
            if (*face_vts[v_ind] == v) {
                opposite_vertex_ind = (v_ind + 1) % 3;
                break;
            }
        }
        neighbour_vertices.push_back(face_vts[opposite_vertex_ind]);
        current_face = current_face->get_adjacent_faces()[opposite_vertex_ind];
    }
    while(current_face != first_face);

    return VertexCirculator(neighbour_vertices);
}


std::ostream& operator<<(std::ostream &strm, const Mesh &m) {
    strm << "===============" << std::endl;
    strm    << "Mesh: " << m.m_vertices.size() << " vertices, "
            << m.m_faces.size() << " faces" << std::endl;
    strm << "Vertices:" << std::endl;
    std::string type;
    VertexConstIteratorType vertex_it;
    FaceConstIteratorType face_it;
    Vertex v;
    Face f;
    glm::vec3 pos;
    std::array<Vertex*, 3> face_vts;
    std::array<Face*, 3> adj_faces;
    for (vertex_it = m.m_vertices.begin(); vertex_it != m.m_vertices.end(); ++vertex_it) {
        v = vertex_it->second;
        pos = v.get_position();
        strm << v.get_hash() << ": ";
        strm << pos.x << " " << pos.y << " " << pos.z;
        strm << " - f=" << v.get_incident_face() << std::endl;
    }
    strm << "Faces:" << std::endl;
    for (face_it = m.m_faces.begin(); face_it != m.m_faces.end(); ++face_it) {
        f = face_it->second;
        face_vts = f.get_vertices();
        adj_faces = f.get_adjacent_faces();
        strm << f.get_hash() << std::endl;
        strm << "    v: "   << face_vts[0]->get_hash() << " "
                            << face_vts[1]->get_hash() << " "
                            << face_vts[2]->get_hash() << std::endl;
        strm << "    a: "   << adj_faces[0]->get_hash() << " "
                            << adj_faces[1]->get_hash() << " "
                            << adj_faces[2]->get_hash() << std::endl;
    }
    strm << "===============" << std::endl;
    return strm;
}

void Mesh::pop_vertex(Vertex *v) {
    m_vertices.erase(v->get_hash());
}

void Mesh::pop_face(Face *f) {
    m_faces.erase(f->get_hash());
}