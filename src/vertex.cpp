#include "vertex.h"

// Represents a vertex in 3D space

Vertex::Vertex (Vector4 position, Vector4 normal)
: m_position(position), m_normal(normal) {}

Vertex::Vertex(float x, float y, float z, float w) {
    m_position = Vector4(x, y, z, w);
    m_normal = Vector4(0, 0, 0, 0);
}

// Perspective divide by w component
Vertex Vertex::perspectiveDivide() {
    float w_inv = 1.0f / getW();
    return Vertex(
        Vector4(
            getX() * w_inv,
            getY() * w_inv,
            getZ() * w_inv,
            getW()
        ),
        m_normal
    );
}

Vector4 Vertex::getPos() {
    return m_position;
}

Vector4 Vertex::getNormal() {
    return m_normal;
}

Vertex Vertex::transform(Matrix4 m, Matrix4 n) {
    return Vertex(m.transform(m_position), n.transform(m_normal));
}

float Vertex::getX() {
    return m_position.x;
}

float Vertex::getY() {
    return m_position.y;
}

float Vertex::getZ() {
    return m_position.z;
}

float Vertex::getW() {
    return m_position.w;
}

void Vertex::setX(float x) {
    m_position.x = x;
}

void Vertex::setY(float y) {
    m_position.y = y;
}

void Vertex::setZ(float z) {
    m_position.z = z;
}

void Vertex::setW(float w) {
    m_position.w = w;
}
