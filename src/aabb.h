#pragma once

// Represents axis-aligned bounding box formed by points minPoint,maxPoint
struct AABB {
    Vector4 minPoint;
    Vector4 maxPoint;

    AABB(Vector4 minPoint_, Vector4 maxPoint_)
    : minPoint(minPoint_), maxPoint(maxPoint_){};
};
