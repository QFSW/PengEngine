#pragma once

#include <math/vector3.h>

namespace physics
{
	// Axis Aligned Bounding Box
	// Defined by the center and the size (half extents) of the box
	struct AABB
	{
		AABB(const math::Vector3f& center, const math::Vector3f& size);

		math::Vector3f center;
		math::Vector3f size;

		[[nodiscard]] bool overlaps(const AABB& other) const;
	};
}