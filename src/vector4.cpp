#include <math.h>
#include "vector4.h"

Vector4::Vector4(float x_, float y_, float z_, float w_)
: x(x_), y(y_), z(z_), w(w_) {}

Vector4 Vector4::operator+(Vector4 b) {
    return Vector4(x + b.x, y + b.y, z + b.z, w + b.w);
}

Vector4 Vector4::operator-(Vector4 b) {
    return Vector4(x - b.x, y - b.y, z - b.z, w - b.w);
}

Vector4 Vector4::operator*(float b) {
    return Vector4(x * b, y * b, z * b, w * b);
}

Vector4 Vector4::operator/(float b) {
    return Vector4(x / b, y / b, z / b, w / b);
}

float Vector4::dot(Vector4 b) {
    return x*b.x + y*b.y + z*b.z + w*b.w;
}

Vector4 Vector4::cross(Vector4 b) {
    return Vector4(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x, w);
}

Vector4& Vector4::norm(Vector4 b) {
    return *this = *this * ( 1 / sqrt(x*x + y*y + z*z + w*w));
}

float Vector4::mag() {
    return sqrt(x*x + y*y + z*z + w*w);

}
