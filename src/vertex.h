#pragma once

#include "vector4.h"
#include "matrix4.h"

class Vertex {
private:
    Vector4 m_position;
public:
    Vertex (Vector4 position=Vector4());
    Vertex (float x=0, float y=0, float z=0, float w=1);
    Vertex perspectiveDivide();
    Vector4 getPos();
    Vertex transform(Matrix4 m);
    float getX();
    float getY();
    float getZ();
    float getW();
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setW(float w);
};
