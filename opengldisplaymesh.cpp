#ifndef OPENGLDISPLAYMESH_H
#define OPENGLDISPLAYMESH_H

#include "mesh.h"


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
        if (isFaceFictive(face_it->idx())) continue;

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
        if (isFaceFictive(face_it->idx())) continue;

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
        if (isFaceFictive(face_it->idx())) continue;

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
}

#endif // OPENGLDISPLAYMESH_H
