#ifndef OPENGLDISPLAYMESH_H
#define OPENGLDISPLAYMESH_H

#include "meshes/mesh.h"
#include "meshes/2d/meshdelaunay.h"
#include "meshes/2d/meshparabola.h"
#include "meshes/2d/meshcrust.h"
#include "meshes/2d/meshruppert.h"


// Draw a vertex
void glVertexDraw(const Vertex & v) {
    const Point p = v.point();
    glVertex3f(p.x(), p.y(), p.z());
}

// Draw mesh with plain faces
void Mesh::drawMesh() {
    // Iteration sur chaque face du mesh
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        // Ne pas afficher les faces ayant un point fictif
        if (!isFaceVisible(face_it->idx())) continue;

        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];

        // Création du triangle
        glBegin(GL_TRIANGLES);
        glColor3d(a.color()[0], a.color()[1], a.color()[2]);
        glVertexDraw(a); // 1er point de la face
        glColor3d(b.color()[0], b.color()[1], b.color()[2]);
        glVertexDraw(b); // 2e point de la face
        glColor3d(c.color()[0], c.color()[1], c.color()[2]);
        glVertexDraw(c); // 3e point de la face
        glEnd();
    }
}

// Draw the wireframe of the mesh
void Mesh::drawMeshWireFrame() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        // Ne pas afficher les faces ayant un point fictif
        if (!isFaceVisible(face_it->idx())) continue;

        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];

        glBegin(GL_LINE_STRIP);
        glColor3d(a.color()[0], a.color()[1], a.color()[2]);
        glVertexDraw(a);
        glColor3d(b.color()[0], b.color()[1], b.color()[2]);
        glVertexDraw(b);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3d(b.color()[0], b.color()[1], b.color()[2]);
        glVertexDraw(b);
        glColor3d(c.color()[0], c.color()[1], c.color()[2]);
        glVertexDraw(c);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3d(c.color()[0], c.color()[1], c.color()[2]);
        glVertexDraw(c);
        glColor3d(a.color()[0], a.color()[1], a.color()[2]);
        glVertexDraw(a);
        glEnd();
    }
}

void Mesh2D::drawMesh() {
    // Iteration sur chaque face du mesh
    for (QVector<Face>::iterator face_it = faceTab.begin(); face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        // Ne pas afficher les faces ayant un point fictif
        if (!isFaceVisible(face_it->idx())) continue;
        //if (isFaceFictive(face_it->idx())) continue;

        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];

        // Création du triangle
        glColor3d(face_it->color()[0], face_it->color()[1], face_it->color()[2]);
        glBegin(GL_TRIANGLES);
        glVertexDraw(a); // 1er point de la face
        glVertexDraw(b); // 2e point de la face
        glVertexDraw(c); // 3e point de la face
        glEnd();
    }
}

// Draw the wireframe of the mesh
void Mesh2D::drawMeshWireFrame() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        // Ne pas afficher les faces ayant un point fictif
        if (!isFaceVisible(face_it->idx())) continue;

        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];

        glColor3d(0, 1, 0);
        glBegin(GL_LINE_STRIP);
        glVertexDraw(a);
        glVertexDraw(b);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertexDraw(b);
        glVertexDraw(c);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertexDraw(c);
        glVertexDraw(a);
        glEnd();
    }

    this->drawVoronoiWireFrame();
}

void Mesh2D::drawVoronoiWireFrame() {
    Circulator_on_faces cf, cfbegin;
    int f1, f2;
    glColor3d(1, 0, 0);
    for (QVector<Vertex>::iterator vertex_it = vertexTab.begin(); vertex_it != vertexTab.end(); ++vertex_it){
        if (!vertex_it->isFictive()) {

            cfbegin = incident_faces(*vertex_it);
            cf = cfbegin;
            do {
                f1 = cf->idx();
                cf++;
                f2 = cf->idx();
                if (!isFaceVisible(f1) || !isFaceVisible(f2)) continue;
                glBegin(GL_LINE_STRIP);
                glVertexDraw(_vVertices[f1]);
                glVertexDraw(_vVertices[f2]);
                glEnd();
            } while (cf != cfbegin);
        }
    }

}

void Crust::drawMeshWireFrame(){
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        // Ne pas afficher les faces ayant un point fictif
        if (!isFaceVisible(face_it->idx())) continue;

        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];

        glColor3d(0, 1, 0);
        if(a.idx() < this->_firstVoronoiIndex && b.idx() < this->_firstVoronoiIndex){
            glBegin(GL_LINE_STRIP);
            glVertexDraw(a);
            glVertexDraw(b);
            glEnd();
        }

        if(c.idx() < this->_firstVoronoiIndex && b.idx() < this->_firstVoronoiIndex){
            glBegin(GL_LINE_STRIP);
            glVertexDraw(b);
            glVertexDraw(c);
            glEnd();
        }

        if(a.idx() < this->_firstVoronoiIndex && c.idx() < this->_firstVoronoiIndex){
            glBegin(GL_LINE_STRIP);
            glVertexDraw(c);
            glVertexDraw(a);
            glEnd();
        }
    }
}
void Parabola::drawMeshWireFrame() {
    Mesh::drawMeshWireFrame();
}

void Parabola::drawMesh() {
    Mesh::drawMesh();
}
void MeshRuppert::drawMesh() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        // Ne pas afficher les faces ayant un point fictif
        if (!isFaceVisible(face_it->idx())) continue;

        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];
        int r = 0;
        int g = 1;
        if(isConstraint(a.idx(), b.idx())){
            r = 1;
            g = 0;
        }
        glBegin(GL_LINE_STRIP);
        glColor3d(r, g, 0);
        glVertexDraw(a);
        glVertexDraw(b);
        glEnd();

        r = 0;
        g = 1;
        if(isConstraint(b.idx(), c.idx())){
            r = 1;
            g = 0;
        }
        glBegin(GL_LINE_STRIP);
        glColor3d(r, g, 0);
        glVertexDraw(b);
        glVertexDraw(c);
        glEnd();

        r = 0;
        g = 1;
        if(isConstraint(c.idx(), a.idx())){
            r = 1;
            g = 0;
        }
        glBegin(GL_LINE_STRIP);
        glColor3d(r, g, 0);
        glVertexDraw(c);
        glVertexDraw(a);
        glEnd();
    }

    for (int i = 0; i < constraint().size(); i++){
        std::array<int, 2> edge = constraint()[i];
        Vertex a, b;
        a = vertexTab[edge[0]];
        b = vertexTab[edge[1]];
        glBegin(GL_LINE_STRIP);
        glColor3d(1.0, 0, 0);
        glVertexDraw(a);
        glColor3d(1.0, 0, 0);
        glVertexDraw(b);
        glEnd();
    }
}
#endif // OPENGLDISPLAYMESH_H
