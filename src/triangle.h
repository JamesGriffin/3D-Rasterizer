#pragma once

#include "vertex.h"
#include "aabb.h"

class Triangle {
private:
    Vertex m_v1;
    Vertex m_v2;
    Vertex m_v3;
public:
    Triangle (Vertex v1, Vertex v2, Vertex v3);
    AABB getAABB();
    Vertex getV1();
    Vertex getV2();
    Vertex getV3();
};
