#ifndef MESH2D_H
#define MESH2D_H

#include <set>

#include "meshes/Mesh.h"


class Mesh2D : public Mesh { // Mesh sur une surface 2D

protected:
    Vertex_Hash m_infinite_vertex;
    std::set<Vertex_Hash> m_hidden_vertices;
    void build_convex_hull();

public:
    Mesh2D();
    virtual ~Mesh2D() {}

    Vertex* get_infinite_vertex();

    int take_step_visibility_march(const Face &face, const Vertex &vtx);
    // Rearrange the triangles of the mesh after the insertion of vtx,
    // so that it corresponds to a Delaunay triangulation.
    void rearrange_delaunay(Vertex &vtx);

    bool is_vertex_fictive(const Vertex &vtx) const;
    bool is_vertex_visible(const Vertex &vtx) const;

    bool is_face_fictive(const Face &face) const;
    bool is_face_visible(const Face &face) const;

    virtual void draw_mesh_faces_colors() override;
    virtual void draw_mesh_wireframe_faces_color() override;

    friend std::ostream& operator<<(std::ostream &strm, const Mesh2D &m);
};

std::ostream& operator<<(std::ostream &strm, const Mesh2D &mesh);

#endif  // MESH2D_H