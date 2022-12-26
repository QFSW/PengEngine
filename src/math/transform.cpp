#include "transform.h"

using namespace math;

Matrix4x4f Transform::to_matrix() const noexcept
{
	return Matrix4x4f::identity()
		.scaled(scale)
		.rotated(rotation)
		.translated(position);
}
