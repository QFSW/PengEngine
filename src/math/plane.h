#pragma once

#include "vector3.h"

namespace math
{
	class Plane
	{
	public:
		Vector3f normal;
		float distance;

		Plane();
		Plane(const Vector3f& normal, float distance);
	};
}
