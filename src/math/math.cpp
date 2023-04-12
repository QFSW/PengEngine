#include "math.h"

#include <cmath>
#include <numbers>

float math::randf()
{
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

math::Vector2f math::rand2f()
{
    return Vector2f(randf(), randf());
}

math::Vector3f math::rand3f()
{
    return Vector3f(randf(), randf(), randf());
}

math::Vector4f math::rand4f()
{
    return Vector4f(randf(), randf(), randf(), randf());
}

float math::rand_range(float min, float max)
{
    return min + randf() * (max - min);
}

float math::map(float val, const Vector2f& in_range, const Vector2f& out_range)
{
    return (val - in_range.x) / (in_range.y - in_range.x) * (out_range.y - out_range.x) + out_range.x;
}

float math::degs_to_rads(float degrees)
{
    return degrees / 180 * std::numbers::pi_v<float>;
}

float math::rads_to_degs(float radians)
{
    return radians * 180 / std::numbers::pi_v<float>;
}
