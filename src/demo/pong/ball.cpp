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
	add_component<MeshRenderer>(
		rendering::Primitives::quad(),
		rendering::Primitives::unlit_material()
	);

	peng::weak_ptr<BoxCollider2D> collider = add_component<BoxCollider2D>();
	collider->triggers_enabled() = true;
	collider->on_trigger_enter().subscribe([this](const peng::weak_ptr<Collider2D>&)
	{
		// TODO: do an actual reflection
		get_component<RigidBody2D>()->velocity *= -1;
	});

	_local_transform.scale = Vector3f(1, 1, 1);
}
