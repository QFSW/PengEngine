#pragma once

#include "vector3.h"

namespace math
{
    class Ray
    {
    public:
        Vector3f origin;
        Vector3f direction;

        Ray(const Vector3f& origin, const Vector3f& direction);
    };
}
