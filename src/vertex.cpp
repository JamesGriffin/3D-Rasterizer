#include "vertex.h"

Vertex::Vertex (Vector4 position)
: m_position(position) {}

Vertex::Vertex(float x, float y, float z, float w) {
    m_position = Vector4(x, y, z, w);
}

Vertex Vertex::perspectiveDivide() {
    float w_inv = 1.0f / getW();
    return Vertex(
        getX() * w_inv,
        getY() * w_inv,
        getZ() * w_inv,
        getW()
    );
}

Vector4 Vertex::getPos() {
    return m_position;
}

Vertex Vertex::transform(Matrix4 m) {
    return Vertex(m.transform(m_position));
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
