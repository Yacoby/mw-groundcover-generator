#include <array>
#include <cmath>

#include "Angle.h"
#include "Vector3.h"


Angle getAngleFromVertical(Angle x, Angle y) {
    float cx = cos(x.asRadians()), sx = sin(x.asRadians());
    float cy = cos(y.asRadians()), sy = sin(y.asRadians());

    Vector3 upVec {-sy, sx * cy, cx * cy };
    Vector3 worldUp = {0, 0, 1};

    float dotProduct = upVec.normalized().dot(worldUp);
    float angleFromVertical = std::acos(dotProduct);
    return Angle::fromRadians(angleFromVertical);
}