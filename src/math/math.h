#pragma once

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
}
