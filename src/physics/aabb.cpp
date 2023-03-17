#include "aabb.h"

using namespace physics;
using namespace math;

AABB::AABB(const Vector3f& center, const Vector3f& size)
	: center(center)
	, size(size)
{ }

bool AABB::overlaps(const AABB& other) const
{
    const Vector3f combined_size = size + other.size;
    const Vector3f piecewise_dist = (center - other.center).abs();

    return
        piecewise_dist.x <= combined_size.x &&
        piecewise_dist.y <= combined_size.y &&
        piecewise_dist.z <= combined_size.z;
}
