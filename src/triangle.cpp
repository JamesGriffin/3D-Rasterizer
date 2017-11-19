#include <algorithm>
#include "triangle.h"
#include "vector4.h"

// Represents triangle formed by vertexes v1,v2,v3
Triangle::Triangle (Vertex v1, Vertex v2, Vertex v3)
: m_v1(v1), m_v2(v2), m_v3(v3) {}

// Return axis-aligned bounding box that conattains triangle
AABB Triangle::getAABB() {
    Vector4 min(
        std::min(std::min(m_v1.getX(), m_v2.getX()), m_v3.getX()),
        std::min(std::min(m_v1.getY(), m_v2.getY()), m_v3.getY()),
        std::min(std::min(m_v1.getZ(), m_v2.getZ()), m_v3.getZ()),
        std::min(std::min(m_v1.getW(), m_v2.getW()), m_v3.getW())
    );

    Vector4 max(
        std::max(std::max(m_v1.getX(), m_v2.getX()), m_v3.getX()),
        std::max(std::max(m_v1.getY(), m_v2.getY()), m_v3.getY()),
        std::max(std::max(m_v1.getZ(), m_v2.getZ()), m_v3.getZ()),
        std::max(std::max(m_v1.getW(), m_v2.getW()), m_v3.getW())
    );

    return AABB(min, max);
}

Vertex Triangle::getV1() {return m_v1;}
Vertex Triangle::getV2() {return m_v2;}
Vertex Triangle::getV3() {return m_v3;}
