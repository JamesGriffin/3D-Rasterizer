#include <math.h>
#include <algorithm>
#include <cstring>
#include <limits>
#include <iostream>
#include "renderer.h"
#include "triangle.h"

// Initalise
Renderer::Renderer(Display& display) : m_display(display) {
    m_zBuffer = new float[display.getWidth() * display.getHeight()];
    clearZBuffer();
};

// Draw line between points x1,y1 and x2,y2 using Bresenham's
// line drawing algorithm
void Renderer::drawLine(float x1, float y1, float x2, float y2, Uint32 color) {

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

// Draw triangle outline defined by vertices v0, v1, v2
void Renderer::drawTriangleOutline(Vertex v0, Vertex v1, Vertex v2, Uint32 color) {
    float w = std::min(v0.getW(), std::min(v1.getW(), v2.getW()));

    if (w < 0) {
        return;
    }

    drawLine(v0.getX(), v0.getY(), v1.getX(), v1.getY(), color);
    drawLine(v0.getX(), v0.getY(), v2.getX(), v2.getY(), color);
    drawLine(v1.getX(), v1.getY(), v2.getX(), v2.getY(), color);
}

// Calculates edge rule for determining if point lies inside a Triangle
// This is also used to calculate barycentric coordinates
float Renderer::orient2D(Vector4 a, Vector4 b, Vector4 c) {
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

// Draw filled triangle defined by vertices v0, v1, v2
// This method utilises barycentric rasterization
void Renderer::drawTriangle(Vertex v0, Vertex v1, Vertex v2, Uint32 color) {

    // Calculate triangle normal for backface culling
    Vector4 s1 = v0.getPos() - v1.getPos();
    Vector4 s2 = v0.getPos() - v2.getPos();

    s1.w = 0;
    s2.w = 0;

    Vector4 norm = s1.cross(s2).norm();
    norm.w = 0;

    // Skip triangle if normal is not facing the camera
    if (norm.dot(Vector4(0, 0, 1.0)) < 0) {
        return;
    }

    // Get triangle axis-aligned bounding box
    Triangle tri(v0, v1, v2);
    AABB box = tri.getAABB();

    int min_x = std::max((int)box.minPoint.x, 0);
    int min_y = std::max((int)box.minPoint.y, 0);
    int max_x = std::min((int)box.maxPoint.x, m_display.getWidth() - 1);
    int max_y = std::min((int)box.maxPoint.y, m_display.getHeight() - 1);

    // Calculate inverse 2*area
    // This is used further below to normalise our barycentric coordinates
    float a = 1.0f / orient2D(v0.getPos(), v1.getPos(), v2.getPos());

    // Vector that represents directional light used for shading
    Vector4 light = Vector4(0.2, -0.3, 1, 0).norm();

    // Calculate shading for each vertex. We multiply by 'a' here to give
    // normalised coordinates when multiplying by w0, w1, w2 further down
    float l0 = -v0.getNormal().norm().dot(light) * a;
    float l1 = -v1.getNormal().norm().dot(light) * a;
    float l2 = -v2.getNormal().norm().dot(light) * a;

    // Calculate highlights for each vertex
    float r0 = a *((v0.getNormal().norm() - light) * -2 * (light.dot(v0.getNormal().norm()))).dot(Vector4(0, 0, -1, 0));
    float r1 = a *((v1.getNormal().norm() - light) * -2 * (light.dot(v1.getNormal().norm()))).dot(Vector4(0, 0, -1, 0));
    float r2 = a *((v2.getNormal().norm() - light) * -2 * (light.dot(v2.getNormal().norm()))).dot(Vector4(0, 0, -1, 0));

    // Loop through pixels in bounding box
    Vector4 p(0, 0);

    for (p.y = min_y; p.y <= max_y; p.y++) {
        for (p.x = min_x; p.x <= max_x; p.x++) {

            // Triangle edge rules
            float w0 = orient2D(v1.getPos(), v2.getPos(), p);
            float w1 = orient2D(v2.getPos(), v0.getPos(), p);
            float w2 = orient2D(v0.getPos(), v1.getPos(), p);

            // Check if pixel lies inside of triangle
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {

                // Calculate interpolated z value for depth culling
                float z = v0.getZ() + (w1 * a)*(v1.getZ() - v0.getZ()) + (w2 * a)*(v2.getZ() - v0.getZ());

                // Check depth against zbuffer
                int Z_buffer_idx = (int)p.y * m_display.getWidth() + (int)p.x;

                if (z < m_zBuffer[Z_buffer_idx]) {

                    // Set new value in zbuffer if point is closer
                    m_zBuffer[Z_buffer_idx] = z;

                    // Combine weighted values to calculate inerpolated shading at pixel.
                    // l0, l1, l2 have already been multipled by inverse 2*area
                    // further up so there is no need to normalise w0, w1, w2
                    float shading = l0*w0 + l1*w1 + l2*w2;

                    shading = 0.9 * shading + (5 * pow(10, -11)) * pow(r0*w0/3 + r1*w1/3 + r2*w2/3, 100);

                    // Clip and add ambient light
                    if (shading < 0.08f) {
                        shading = 0.08f;
                    }
                    if (shading > 1.0f) {
                        shading = 1.0f;
                    }

                    // Draw pixel to display
                    m_display.drawPixelFast((int)ceilf(p.x), (int)ceilf(p.y),
                        (255 << 24) | (int(shading * 255+0.5f) << 16) | (int(shading *255+0.5f) << 8) | (int(shading *255+0.5f)));

                }
            }
        }
    }
}

// Draw transformed mesh
void Renderer::drawMesh(Mesh mesh, Matrix4 transform, Uint32 color, bool fill) {
    // Initialise screenspace transform
    Matrix4 ss_transform = Matrix4::initScreenSpaceTransform(
        m_display.getWidth(), m_display.getHeight()
    );
    // Initialise perspective transform
    Matrix4 persp_transform = Matrix4::initPerspective(
        70,
        (float)m_display.getWidth() / (float)m_display.getHeight(),
        0.1f, 1000.0f
    );
    // Multiply to create final transformation matrices
    Matrix4 normal_transform = transform;
    transform = ss_transform.mul(persp_transform).mul(transform);

    // Loop over shapes in mesh file
    for (size_t s = 0; s < mesh.shapes.size(); s++) {
        // Loop over faces(polygons) in shape
        #pragma omp parallel for schedule(dynamic, 3)
        for (size_t f = 0; f < mesh.shapes[s].mesh.indices.size(); f+=3) {

            // Create vertex instances that form triangle and apply transformations

            // Vertex 0
            tinyobj::index_t idx = mesh.shapes[s].mesh.indices[f];
            Vertex v0 = Vertex(
                Vector4(
                    mesh.attrib.vertices[3*idx.vertex_index+0],
                    mesh.attrib.vertices[3*idx.vertex_index+1],
                    mesh.attrib.vertices[3*idx.vertex_index+2]
                ),
                Vector4(
                    mesh.attrib.normals[3*idx.normal_index+0],
                    mesh.attrib.normals[3*idx.normal_index+1],
                    mesh.attrib.normals[3*idx.normal_index+2],
                    0
                )
            ).transform(transform, normal_transform).perspectiveDivide();
            // Vertex 1
            idx = mesh.shapes[s].mesh.indices[f+1];
            Vertex v1= Vertex(
                Vector4(
                    mesh.attrib.vertices[3*idx.vertex_index+0],
                    mesh.attrib.vertices[3*idx.vertex_index+1],
                    mesh.attrib.vertices[3*idx.vertex_index+2]
                ),
                Vector4(
                    mesh.attrib.normals[3*idx.normal_index+0],
                    mesh.attrib.normals[3*idx.normal_index+1],
                    mesh.attrib.normals[3*idx.normal_index+2],
                    0
                )
            ).transform(transform, normal_transform).perspectiveDivide();
            // Vertex 2
            idx = mesh.shapes[s].mesh.indices[f+2];
            Vertex v2= Vertex(
                Vector4(
                    mesh.attrib.vertices[3*idx.vertex_index+0],
                    mesh.attrib.vertices[3*idx.vertex_index+1],
                    mesh.attrib.vertices[3*idx.vertex_index+2]
                ),
                Vector4(
                    mesh.attrib.normals[3*idx.normal_index+0],
                    mesh.attrib.normals[3*idx.normal_index+1],
                    mesh.attrib.normals[3*idx.normal_index+2],
                    0
                )
            ).transform(transform, normal_transform).perspectiveDivide();

            // Draw triangle
            if (fill) {
                drawTriangle(v0, v1, v2, color);
            }
            else {
                drawTriangleOutline(v2, v1, v0, color);
            }
        }
    }
}

void Renderer::drawZBuffer() {
    for (int x = 0; x < m_display.getWidth(); x++) {
        for (int y = 0; y < m_display.getHeight(); y++) {
            float shading = ( 1 + m_zBuffer[y * m_display.getWidth() + x]) / 2;
            if (shading < 0) {
                shading = 0;
            }
            m_display.drawPixelFast(x, y, (255 << 24) | (int(shading * 255+0.5f) << 16) | (int(shading * 255+0.5f) << 8) | (int(shading * 255+0.5f)));
        }
    }
}

// Reset zbuffer to default values
void Renderer::clearZBuffer() {
    for (int i = 0; i < m_display.getWidth() * m_display.getHeight(); i++) {
        m_zBuffer[i] = std::numeric_limits<float>::infinity();
    }
}

// Clean up
Renderer::~Renderer(){
    delete[] m_zBuffer;
}
