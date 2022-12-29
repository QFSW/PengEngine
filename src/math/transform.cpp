#include "transform.h"

using namespace math;

Transform::Transform()
	: position(Vector3f::zero())
	, scale(Vector3f::one())
	, rotation(Vector3f::zero())
{ }

Transform::Transform(const Vector3f& position, const Vector3f& scale, const Vector3f& rotation)
	: position(position)
	, scale(scale)
	, rotation(rotation)
{ }

Matrix4x4f Transform::to_matrix() const noexcept
{
	return Matrix4x4f::identity()
		.scaled(scale)
		.rotated(rotation)
		.translated(position);
}

Matrix4x4f Transform::to_inverse_matrix() const noexcept
{
	return Matrix4x4f::identity()
		.translated(-position)
		.rotated(-rotation)
		.scaled(scale.reciprocal());
}
