#pragma once

#include "vector3.h"
#include "matrix4x4.h"

namespace math
{
	struct Transform
	{
		Vector3f position;
		Vector3f scale;
		Vector3f rotation;

		[[nodiscard]] Matrix4x4f to_matrix() const noexcept;
		[[nodiscard]] Matrix4x4f to_inverse_matrix() const noexcept;
	};
}
