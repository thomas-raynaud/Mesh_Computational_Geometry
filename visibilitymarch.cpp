#include "mesh.h"

int Mesh2D::takeStepVisibilityMarch(const int& faceIdx, const int& vertexIdx) {
    Point a, b;
    double nextFaceIdx = -1;
    for (int i = 0; i < 3; ++i) {
        a = vertexTab[faceTab[faceIdx].vertices()[i]].point();
        b = vertexTab[faceTab[faceIdx].vertices()[(i + 1) % 3]].point();
        if (testOrientation(a, b, vertexTab[vertexIdx].point()) < 0) {
            nextFaceIdx = faceTab[faceIdx].adjacentFaces()[(i + 2) % 3];
            break;
        }
    }
    return nextFaceIdx;
}
