#include "ball.h"

#include <components/mesh_renderer.h>
#include <components/rigid_body_2d.h>
#include <components/box_collider_2d.h>
#include <rendering/primitives.h>
#include <math/math.h>

IMPLEMENT_ENTITY(demo::pong::Ball);

using namespace demo::pong;
using namespace components;
using namespace math;

Ball::Ball()
	: Entity("Ball")
{
	const Vector2f velocity = Vector2f(rand_range(-1, 1), rand_range(-1, 1)).normalized() * 20;

	add_component<RigidBody2D>()->velocity = velocity;
	add_component<BoxCollider2D>();
	add_component<MeshRenderer>(
		rendering::Primitives::quad(),
		rendering::Primitives::unlit_material()
	);

	_local_transform.scale = Vector3f(1, 1, 1);
}

void Ball::tick(float delta_time)
{
	Entity::tick(delta_time);

	peng::weak_ptr<Collider2D> this_collider = get_component<BoxCollider2D>();
	const physics::AABB this_aabb = this_collider->bounding_box();

	for (peng::weak_ptr<Collider2D> other_collider : Collider2D::active_colliders())
	{
		if (this_collider != other_collider)
		{
			const physics::AABB other_aabb = other_collider->bounding_box();
			if (this_aabb.overlaps(other_aabb))
			{
				// TODO: do an actual reflection
				// TODO: can get stuck, move it outside the collider
				get_component<RigidBody2D>()->velocity *= -1;
				break;
			}
		}
	}
}
