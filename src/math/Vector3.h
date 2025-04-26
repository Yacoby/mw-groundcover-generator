#pragma once

#include <cmath>

struct Vector3 {
    float x;
    float y;
    float z;

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 normalized() const {
        float mag = magnitude();
        return {x / mag, y / mag, z / mag};
    }
};