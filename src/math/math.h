#pragma once

#include "number.h"
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

namespace math
{
    float randf();
    Vector2f rand2f();
    Vector3f rand3f();
    Vector4f rand4f();

    float rand_range(float min, float max);
    float map(float val, const Vector2f& in_range, const Vector2f& out_range);

    template <number T>
    T sgn(T val)
    {
        return static_cast<T>(T(0) < val) - (val < T(0));
    }

    template <number T>
    T sqrt_signed(T val)
    {
        return std::sqrt(std::abs(val)) * sgn(val);
    }

    template <number T>
    T sqr(T val)
    {
        return val * val;
    }
}
