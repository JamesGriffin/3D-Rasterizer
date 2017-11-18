#pragma once

#include "vector4.h"

class Matrix4 {
private:
    float m_matrix[4][4];
public:
    Matrix4();
    Matrix4(float matrix[][4]);
    void setMatrix(float matrix[][4]);
    float get(int i, int j);
    static Matrix4 initScreenSpaceTransform(float width, float height);
    static Matrix4 initPerspective(float fov, float aspectRatio, float zNear,
        float zFar);
    static Matrix4 initTranslation(float x, float y, float z);
    static Matrix4 initRotation(float x, float y, float z);
    static Matrix4 initScale(float x, float y, float z);
    Vector4 transform(Vector4 r);
    Matrix4 mul(Matrix4 r);
};
