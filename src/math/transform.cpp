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

Vector3f Transform::local_right() const noexcept
{
	// TODO: breaks if local_forwards == up
	return Vector3f::cross(Vector3f::up(), local_forwards());
}

Vector3f Transform::local_up() const noexcept
{
	// TODO: breaks if local_forwards == right
	return Vector3f::cross(Vector3f::right(), local_forwards());
}

Vector3f Transform::local_forwards() const noexcept
{
	const float pitch_rads = rotation.x * std::numbers::pi_v<float> / 180;
	const float yaw_rads = rotation.y * std::numbers::pi_v<float> / 180;

	return Vector3f(
		std::sin(yaw_rads) * std::cos(pitch_rads),
		-std::sin(pitch_rads),
		std::cos(yaw_rads) * std::cos(pitch_rads)
	);
}
