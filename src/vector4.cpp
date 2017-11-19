#include <math.h>
#include "vector4.h"

// General purpose Vector4 class used for points and vectors
Vector4::Vector4(float x_, float y_, float z_, float w_)
: x(x_), y(y_), z(z_), w(w_) {}

// Addition with vector
Vector4 Vector4::operator+(Vector4 b) {
    return Vector4(x + b.x, y + b.y, z + b.z, w + b.w);
}
// Subtraction with vector
Vector4 Vector4::operator-(Vector4 b) {
    return Vector4(x - b.x, y - b.y, z - b.z, w - b.w);
}

// Multiplcation with scalar
Vector4 Vector4::operator*(float b) {
    return Vector4(x * b, y * b, z * b, w * b);
}

// Division with scalar
Vector4 Vector4::operator/(float b) {
    return Vector4(x / b, y / b, z / b, w / b);
}

// Dot product
float Vector4::dot(Vector4 b) {
    return x*b.x + y*b.y + z*b.z + w*b.w;
}

// Cross product (w component is ignored)
Vector4 Vector4::cross(Vector4 b) {
    return Vector4(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x, w);
}

// Normalise to form unit vector
Vector4& Vector4::norm() {
    return *this = *this * ( 1 / sqrt(x*x + y*y + z*z + w*w));
}

// Calculate vector magnitude
float Vector4::mag() {
    return sqrt(x*x + y*y + z*z + w*w);

}
