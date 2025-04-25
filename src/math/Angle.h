#pragma once

#include "Constants.h"

class Angle {
private:
    double angleInRadians;
    explicit Angle(double radians) : angleInRadians(radians) {}
public:
    static Angle fromRadians(double radians) {
        return Angle(radians);
    }

    static Angle fromDegrees(double degrees) {
        return Angle(degrees * M_PI / 180.0);
    }

    double asRadians() const {
        return angleInRadians;
    }

    double asDegrees() const {
        return angleInRadians * 180 / M_PI;
    }
};