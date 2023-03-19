#include "paddle.h"

#include <components/sprite_renderer.h>
#include <components/rigid_body_2d.h>
#include <components/box_collider_2d.h>
#include <rendering/primitives.h>
#include <math/math.h>

IMPLEMENT_ENTITY(demo::pong::Paddle);

using namespace demo::pong;
using namespace components;
using namespace math;

Paddle::Paddle(const std::string& name)
	: Entity(name)
{
	add_component<RigidBody2D>();
	add_component<SpriteRenderer>();

	peng::weak_ptr<Collider2D> collider = add_component<BoxCollider2D>();
	collider->triggers_enabled = true;
	collider->layer = physics::Layer(1);
	collider->on_trigger_stay().subscribe([this](peng::weak_ptr<Collider2D> other)
		{
			handle_collision(other);
		});

	_local_transform.scale = Vector3f(1, 7, 1);
}

void Paddle::tick(float delta_time)
{
	Entity::tick(delta_time);

	const float up_amount = PengEngine::get().input_manager().axis_value(input_axis);
	get_component<RigidBody2D>()->velocity = Vector2f::up() * up_amount * movement_speed;
}

void Paddle::score_point()
{
	_score++;
	_on_score_changed(_score);
}

void Paddle::handle_collision(peng::weak_ptr<Collider2D> collider)
{
	if (collider->layer == physics::Layer(0))
	{
		const physics::AABB aabb = get_component<BoxCollider2D>()->bounding_box();
		const physics::AABB other_aabb = collider->bounding_box();

		const Vector3f dist = other_aabb.center - aabb.center;
		const Vector3f desired_dist = other_aabb.size + aabb.size;

		_local_transform.position.y = (other_aabb.center - desired_dist * sgn(dist.y)).y;
		get_component<RigidBody2D>()->velocity = Vector2f::zero();
	}
}
