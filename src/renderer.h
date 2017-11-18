#pragma once

#include "display.h"
#include "vertex.h"

class Renderer {
private:
    Display& m_display;
public:
    Renderer (Display& display);
    void drawLine(float x1, float y1, float x2, float y2, Uint32 color);
    void drawTriangleOutline(Vertex v1, Vertex v2, Vertex v3, Uint32 color);
};
