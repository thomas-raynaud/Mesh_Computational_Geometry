#ifndef OPENGLDISPLAYMESH_H
#define OPENGLDISPLAYMESH_H

#include "mesh.h"


// Draw a vertex
void glVertexDraw(const Vertex & v) {
    const Point p = v.point();
    glVertex3f(p.x(), p.y(), p.z());
}

//Example with a tetraedra
void Mesh::drawMesh() {
    //getLap();
    int color_ind = 0;
    // Iteration sur chaque face du mesh
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        if (faceVertices[0] == -1 || faceVertices[1] == -1 || faceVertices[2] == -1) continue;
        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];
        std::array<double, 3> rgb = face_it->color();

        // Couleur
        /*switch(color_ind % 6) {
            case 0: glColor3d(1,0,0); break; // Red
            case 1: glColor3d(0,1,0); break; // Green
            case 2: glColor3d(0,0,1); break; // Blue
            case 3: glColor3d(0.8, 0, 0.8); break;
            case 4: glColor3d(0.4, 0.2, 0.8); break;
            default: glColor3d(1,1,0); break; // Yellow
        }*/
        glColor3d(rgb[0], rgb[1], rgb[2]);
        // Création du triangle
        glBegin(GL_TRIANGLES);
        glColor3d(1, 0, 0);
        glVertexDraw(a); // 1er point de la face
        glColor3d(0, 1, 0);
        glVertexDraw(b); // 2e point de la face
        glColor3d(0, 0, 1);
        glVertexDraw(c); // 3e point de la face
        glEnd();

        color_ind++;
    }
}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    for (QVector<Face>::iterator face_it = faceTab.begin() ; face_it != faceTab.end(); ++face_it) {
        Vertex a, b, c;
        std::array<int, 3> faceVertices = face_it->vertices();
        if (faceVertices[0] == -1 || faceVertices[1] == -1 || faceVertices[2] == -1) continue;
        a = vertexTab[faceVertices[0]];
        b = vertexTab[faceVertices[1]];
        c = vertexTab[faceVertices[2]];
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
