#include "mesh2d.h"


std::ostream& operator<<(std::ostream &strm, const Mesh2D &m) {
    strm << "===============" << std::endl;
    strm << "Mesh: " << m.m_vertices.size() << " vertices, " << m.faceTab.size() << " faces" << std::endl;
    strm << "\nVertices:\n";
    std::string type;
    for (int i = 0; i < m.m_vertices.size(); ++i) {
        if (m.is_vertex_fictive(m.m_vertices[i]))
            type = " (fictive)";
        else if (!m.is_vertex_visible(m.m_vertices[i]))
            type = " (hidden)";
        else
            type = "";
        strm << m.m_vertices[i].idx() << ": "
                          << m.m_vertices[i].point().x() << " "
                          << m.m_vertices[i].point().y() << " "
                          << m.m_vertices[i].point().z() << type << " - f="
                          << m.m_vertices[i].face() << std::endl;
    }
    strm << "Faces:\n";
    for (int i = 0; i < m.faceTab.size(); ++i) {
        if (m.isFaceFictive(m.faceTab[i].idx()))
            type = " (fictive)";
        else if (!m.isFaceVisible(m.faceTab[i].idx()))
            type = " (hidden)";
        else
            type = "";
        strm << m.faceTab[i].idx() << type << ":\n";
        strm << "   v: " << m.faceTab[i].vertices()[0] << " "
                        << m.faceTab[i].vertices()[1] << " "
                        << m.faceTab[i].vertices()[2] << std::endl;
        strm << "   a: " << m.faceTab[i].adjacentFaces()[0] << " "
                        << m.faceTab[i].adjacentFaces()[1] << " "
                        << m.faceTab[i].adjacentFaces()[2] << std::endl;
    }
    strm << "===============" << std::endl;
    return strm;
}