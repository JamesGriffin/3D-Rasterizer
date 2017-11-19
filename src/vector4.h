#pragma once

struct Vector4 {
    float x;
    float y;
    float z;
    float w;
    
    Vector4 (float x_=0, float y_=0, float z_=0, float w_=1);
    Vector4 operator+(Vector4 b);
    Vector4 operator-(Vector4 b);
    Vector4 operator*(float b);
    Vector4 operator/(float b);
    float dot(Vector4 b);
    Vector4 cross(Vector4 b);
    Vector4& norm();
    float mag();
};
