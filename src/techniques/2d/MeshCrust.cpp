#include "MeshCrust.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "delaunay.h"
#include "voronoi.h"
#include "utils/read_3d_file.h"


MeshCrust::MeshCrust() : Mesh2D() {
    Data3D crust_data;
    std::string filename = "resources/crust.obj";
    float x, y;
    Vertex *vtx;
    // Read OBJ file
    read_3d_file(filename, &crust_data);
    // Insert vertices from OBJ data
    for (size_t i = 0; i < crust_data.vertices.size(); ++i) {
        vtx = delaunay::insert_vertex(
            *this, glm::vec3(crust_data.vertices[i][0], crust_data.vertices[i][1], 0.f)
        );
        m_is_voronoi_vertex[vtx->get_hash()] = false;
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