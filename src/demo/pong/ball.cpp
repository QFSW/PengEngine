#include "ball.h"

#include <components/mesh_renderer.h>
#include <components/rigid_body_2d.h>
#include <components/box_collider_2d.h>
#include <rendering/primitives.h>
#include <math/math.h>

#include "paddle.h"

IMPLEMENT_ENTITY(demo::pong::Ball);

using namespace demo::pong;
using namespace components;
using namespace math;

Ball::Ball()
	: Entity("Ball")
	, _speed(30)
{
	const Vector2f velocity = Vector2f(rand_range(-1, 1), rand_range(-1, 1)).normalized() * _speed;

	add_component<RigidBody2D>()->velocity = velocity;
	add_component<MeshRenderer>(
		rendering::Primitives::quad(),
		rendering::Primitives::unlit_material()
	);

	peng::weak_ptr<BoxCollider2D> collider = add_component<BoxCollider2D>();
	collider->triggers_enabled() = true;
	collider->on_trigger_enter().subscribe([this](const peng::weak_ptr<Collider2D>& collider)
		{
			handle_collision(collider);
		});

	_local_transform.scale = Vector3f(1, 1, 1);
}

void Ball::handle_collision(peng::weak_ptr<Collider2D> collider)
{
	const physics::AABB box = collider->bounding_box();
	const Vector3f delta = box.center - world_position();

	if (peng::weak_ptr<const Paddle> paddle = collider->owner().as_type<Paddle>())
	{
		// If it's a paddle, we need to bounce away in an arc based on the vertical hit position
		constexpr float to_rads = 3.14f / 180.0f;
		const float y_norm = -delta.y / box.size.y;
		const float angle = y_norm * paddle->attack_arc / 2;
		Vector2f dir = Vector2f(std::cos(angle * to_rads), std::sin(angle * to_rads));
		if (delta.x > 0)
		{
			dir.x *= -1;
		}

		get_component<RigidBody2D>()->velocity = dir * _speed;
	}
	else
	{
		const float box_aspect = box.size.x / box.size.y;

		Vector3f dist = delta.abs();
		dist.x /= box_aspect;

		const Vector2f reflector = Vector2f(
			dist.x >= dist.y ? -1.0f : 1.0f,
			dist.y >= dist.x ? -1.0f : 1.0f
		);

		get_component<RigidBody2D>()->velocity *= reflector;
	}
}
