#include "MeshCrust.h"

#include <fstream>
#include <sstream>

#include "delaunay.h"
#include "voronoi.h"


MeshCrust::MeshCrust() : Mesh2D() {
    // Insert points in the file.
    std::ifstream file("resources/points_courbe.txt");
    if(!file) return;
    std::string line;
    float x, y;
    std::istringstream iss;
    Vertex *vtx;
    while (!file.eof()) {
        std::getline(file, line);
        iss.str(line);
        iss >> x >> y;
        vtx = delaunay::insert_vertex(*this, glm::vec3(x, y, 0.f));
        m_is_voronoi_vertex[vtx->get_hash()] = false;
        iss.clear();
    }
    update_hidden_vertices();

    // Insert the Voronoi centers in the mesh
    std::unordered_map<Face_Hash, glm::vec3> voronoi_vts = build_voronoi(this);
    std::unordered_map<Face_Hash, glm::vec3>::iterator voronoi_it;
    for (voronoi_it = voronoi_vts.begin(); voronoi_it != voronoi_vts.end(); ++voronoi_it) {
        vtx = delaunay::insert_vertex(*this, voronoi_it->second);
        m_is_voronoi_vertex[vtx->get_hash()] = true;
    }
}

BoundingBox MeshCrust::get_bounding_box() {
    BoundingBox bb;
    std::unordered_map<Vertex_Hash, Vertex>::iterator vtx_it;
    vtx_it = m_vertices.begin();
    while (vtx_it != m_vertices.end() && !is_vertex_visible(vtx_it->second)) {
        ++vtx_it;
    }
    if (vtx_it == m_vertices.end()) {
        return Mesh::get_bounding_box();
    }
    else {
        bb.min = vtx_it->second.get_position();
        bb.max = vtx_it->second.get_position();
        for (vtx_it = m_vertices.begin(); vtx_it != m_vertices.end(); ++vtx_it) {
            if (!is_vertex_visible(vtx_it->second) || is_voronoi_vertex(vtx_it->second))
                continue;
            bb.min = glm::min(bb.min, vtx_it->second.get_position());
            bb.max = glm::max(bb.max, vtx_it->second.get_position());
        }
        return bb;
    }
}

bool MeshCrust::is_voronoi_vertex(const Vertex &vtx) {
    return m_is_voronoi_vertex[vtx.get_hash()];
}