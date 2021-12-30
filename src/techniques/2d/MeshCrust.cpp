#include "MeshCrust.h"

#include <fstream>
#include <sstream>

#include "delaunay.h"
#include "voronoi.h"


MeshCrust::MeshCrust(){
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
        vtx = insert_delaunay_vertex(this, glm::vec3(x, y, 0.f));
        m_is_voronoi_vertex[vtx->get_hash()] = false;
        iss.clear();
    }

    // Insert the Voronoi centers in the mesh
    std::unordered_map<Face_Hash, Vertex> voronoi_vts = build_voronoi(this);
    std::unordered_map<Face_Hash, Vertex>::iterator vtx_it;
    for (vtx_it = voronoi_vts.begin(); vtx_it != voronoi_vts.end(); ++vtx_it) {
        vtx = insert_delaunay_vertex(this, vtx->get_position());
        m_is_voronoi_vertex[vtx->get_hash()] = true;
    }
}

bool MeshCrust::is_voronoi_vertex(const Vertex &vtx) {
    return m_is_voronoi_vertex[vtx.get_hash()];
}