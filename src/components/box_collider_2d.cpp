#include "box_collider_2d.h"

IMPLEMENT_COMPONENT(components::BoxCollider2D);

using namespace components;
using namespace math;

physics::AABB BoxCollider2D::bounding_box() const
{
	const Vector3f p1 = Vector3f(-0.5f, -0.5f, -1);
	const Vector3f p2 = Vector3f(0.5f, 0.5f, 1);

	const Matrix4x4f transform = owner().transform_matrix();
	const Vector3f p1_m = transform * p1;
	const Vector3f p2_m = transform * p2;

	const Vector3f center = (p1_m + p2_m) / 2;
	const Vector3f size = (p2_m - p1_m) / 2;

	return physics::AABB(center, size);
}
