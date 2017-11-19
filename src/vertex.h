#pragma once

#include "vector4.h"
#include "matrix4.h"

class Vertex {
private:
    Vector4 m_position;
    Vector4 m_normal;
public:
    Vertex (Vector4 position=Vector4(), Vector4 normal=Vector4(0, 0, 0, 0));
    Vertex (float x=0, float y=0, float z=0, float w=1);
    Vertex perspectiveDivide();
    Vector4 getPos();
    Vector4 getNormal();
    Vertex transform(Matrix4 m, Matrix4 n=Matrix4());
    float getX();
    float getY();
    float getZ();
    float getW();
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setW(float w);
};
