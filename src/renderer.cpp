#include "renderer.h"

Renderer::Renderer(Display& display) : m_display(display) {};

void Renderer::drawLine(float x1, float y1, float x2, float y2, Uint32 color) {
    // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep) {
      std::swap(x1, y1);
      std::swap(x2, y2);
    }

    if(x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++) {
        if(steep) {
            m_display.drawPixelFast(y, x, color);
        }
        else {
            m_display.drawPixelFast(x, y, color);
        }

        error -= dy;
        if(error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void Renderer::drawTriangleOutline(Vertex v1, Vertex v2, Vertex v3, Uint32 color) {
    drawLine(v1.getX(), v1.getY(), v2.getX(), v2.getY(), color);
    drawLine(v1.getX(), v1.getY(), v3.getX(), v3.getY(), color);
    drawLine(v2.getX(), v2.getY(), v3.getX(), v3.getY(), color);
}
