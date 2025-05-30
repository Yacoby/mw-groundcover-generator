#pragma once

#include <ostream>

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

    bool operator <(const Angle& rhs) const {
        return (angleInRadians < rhs.angleInRadians);
    }

    bool operator <=(const Angle& rhs) const {
        return (angleInRadians <= rhs.angleInRadians);
    }

    bool operator >(const Angle& rhs) const {
        return (angleInRadians > rhs.angleInRadians);
    }

    bool operator >=(const Angle& rhs) const {
        return (angleInRadians >= rhs.angleInRadians);
    }

    bool operator ==(const Angle& rhs) const {
        return (angleInRadians == rhs.angleInRadians);
    }

    friend std::ostream& operator<<(std::ostream& os, const Angle& angle) {
        os << "{Angle:" << angle.angleInRadians << "}";
        return os;
    }
};