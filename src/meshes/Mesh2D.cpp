#include "Mesh2D.h"

#include <iostream>


Mesh2D::Mesh2D() {
    // Create an invisible tetrahedron that wraps up the 2D mesh.
    Vertex va(glm::vec3(-MAX_DIST,       0.f, 0.f));
    Vertex vb(glm::vec3( MAX_DIST, -MAX_DIST, 0.f));
    Vertex vc(glm::vec3( MAX_DIST,  MAX_DIST, 0.f));
    Vertex inf_vtx(glm::vec3(0.f, 0.f, -1.f));
    m_vertices[va.get_hash()] = va;
    m_vertices[vb.get_hash()] = vb;
    m_vertices[vc.get_hash()] = vc;
    m_vertices[inf_vtx.get_hash()] = inf_vtx;
    m_infinite_vertex = inf_vtx.get_hash();
    m_hidden_vertices.insert({
        va.get_hash(),
        vb.get_hash(),
        vc.get_hash(),
        inf_vtx.get_hash()
    });
    Face fa({ &va, &vb,      &vc      });
    Face fb({ &vb, &inf_vtx, &vc      });
    Face fc({ &va, &vc,      &inf_vtx });
    Face fd({ &va, &inf_vtx, &vb      });
    m_faces[fa.get_hash()] = fa;
    m_faces[fb.get_hash()] = fb;
    m_faces[fc.get_hash()] = fc;
    m_faces[fd.get_hash()] = fd;
    connect_adjacent_faces();
}


void Mesh2D::build_convex_hull() {
}


Vertex* Mesh2D::get_infinite_vertex() {
    return &m_vertices[m_infinite_vertex];
}


bool Mesh2D::is_vertex_fictive(const Vertex &vtx) const {
    return m_infinite_vertex == vtx.get_hash();
}

bool Mesh2D::is_vertex_visible(const Vertex &vtx) const {
    return m_hidden_vertices.find(vtx.get_hash()) == m_hidden_vertices.end();
}

bool Mesh2D::is_face_fictive(const Face &face) const {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    return  is_vertex_fictive(*face_vts[0]) ||
            is_vertex_fictive(*face_vts[1]) ||
            is_vertex_fictive(*face_vts[2]);
}

bool Mesh2D::is_face_visible(const Face &face) const {
    std::array<Vertex*, 3> face_vts = face.get_vertices();
    return  is_vertex_visible(*face_vts[0]) ||
            is_vertex_visible(*face_vts[1]) ||
            is_vertex_visible(*face_vts[2]);
}


std::ostream& operator<<(std::ostream &strm, const Mesh2D &mesh) {
    strm << "===============" << std::endl;
    strm    << "Mesh: " << mesh.m_vertices.size() << " vertices, "
            << mesh.m_faces.size() << " faces" << std::endl;
    strm << "Vertices:" << std::endl;
    std::string type_str;
    VertexConstIteratorType vertex_it;
    FaceConstIteratorType face_it;
    Vertex v;
    Face f;
    glm::vec3 pos;
    std::array<Vertex*, 3> face_vts;
    std::array<Face*, 3> adj_faces;
    for (   vertex_it = mesh.m_vertices.begin();
            vertex_it != mesh.m_vertices.end();
            ++vertex_it
    ) {
        v = vertex_it->second;
        if (mesh.is_vertex_fictive(v))
            type_str = " (fictive)";
        else if (!mesh.is_vertex_visible(v))
            type_str = " (hidden)";
        else
            type_str = "";
        pos = v.get_position();
        strm << v.get_hash() << ": ";
        strm << pos.x << " " << pos.y << " " << pos.z;
        strm << type_str;
        strm << " - f=" << v.get_incident_face() << std::endl;
    }
    strm << "Faces:" << std::endl;
    for (   face_it = mesh.m_faces.begin();
            face_it != mesh.m_faces.end();
            ++face_it
    ) {
        if (mesh.is_face_fictive(f))
            type_str = " (fictive)";
        else if (!mesh.is_face_visible(f))
            type_str = " (hidden)";
        else
            type_str = "";
        f = face_it->second;
        face_vts = f.get_vertices();
        adj_faces = f.get_adjacent_faces();
        strm << f.get_hash()  << type_str << ": " << std::endl;
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