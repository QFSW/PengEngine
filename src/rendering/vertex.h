#pragma once

#include <math/vector3.h>

namespace rendering
{
	struct Vertex
	{
		math::Vector3f position;
		math::Vector3f color;
		math::Vector2f tex_coord;
	};
}
