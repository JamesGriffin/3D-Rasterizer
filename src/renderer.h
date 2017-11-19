#pragma once

#include "display.h"
#include "vertex.h"
#include "mesh.h"
#include "matrix4.h"

class Renderer {
private:
    Display& m_display;
    float* m_zBuffer;
public:
    Renderer (Display& display);
    void drawLine(float x1, float y1, float x2, float y2, Uint32 color);
    void drawTriangleOutline(Vertex v0, Vertex v1, Vertex v2, Uint32 color);
    void drawTriangle(Vertex v0, Vertex v1, Vertex v2, Uint32 color);
    void drawMesh(Mesh mesh, Matrix4 transform, Uint32 color);
    void clearZBuffer();
    virtual ~Renderer();
};
