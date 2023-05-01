#include "ball.h"

#include <audio/audio_clip.h>
#include <core/serialized_member.h>
#include <core/asset.h>
#include <components/sprite_renderer.h>
#include <components/rigid_body_2d.h>
#include <components/box_collider_2d.h>
#include <rendering/primitives.h>
#include <math/math.h>

#include "goal.h"
#include "paddle.h"

IMPLEMENT_ENTITY(demo::pong::Ball);

using namespace demo::pong;
using namespace components;
using namespace audio;
using namespace math;

Ball::Ball()
	: Entity("Ball")
	, _speed(50)
{
	SERIALIZED_MEMBER(_speed);
	SERIALIZED_MEMBER(_bounce_wall_sfx);
	SERIALIZED_MEMBER(_bounce_paddle_sfx);
	SERIALIZED_MEMBER(_goal_sfx);

	add_component<RigidBody2D>();
	add_component<SpriteRenderer>();

	peng::weak_ptr<BoxCollider2D> collider = add_component<BoxCollider2D>();
	collider->triggers_enabled = true;
	collider->layer = physics::Layer(2);
	collider->on_trigger_enter().subscribe([this](const peng::weak_ptr<Collider2D>& collider)
		{
			handle_collision(collider);
		});

	_local_transform.scale = Vector3f(1, 1, 1);
	respawn();
}

void Ball::respawn()
{
	const float angle = rand_range(-1, 1);
	const Vector2f dir = Vector2f(std::cos(angle), std::sin(angle));
	const Vector2f reflector = Vector2f(rand() % 2 ? -1 : 1, 1);
	const Vector2f velocity = dir * reflector * _speed * 0.75f;

	get_component<RigidBody2D>()->velocity = velocity;
	_local_transform.position = Vector3f::zero();
}

void Ball::handle_collision(peng::weak_ptr<Collider2D> collider)
{
	const physics::AABB box = collider->bounding_box();
	const Vector3f delta = box.center - world_position();

	if (peng::weak_ptr<const Goal> goal = collider->owner().as_type<Goal>())
	{
		// If it's a goal, increment score and respawn
		goal->associated_paddle()->score_point();
		if (_goal_sfx)
		{
			_audio_pool.play(_goal_sfx.to_shared_ref());
		}

		respawn();
	}
	else if (peng::weak_ptr<const Paddle> paddle = collider->owner().as_type<Paddle>())
	{
		// If it's a paddle, we need to bounce away in an arc based on the vertical hit position
		constexpr float to_rads = 3.14f / 180.0f;
		const float y_norm = -delta.y / box.size.y;
		const float angle = y_norm * paddle->attack_arc / 2;
		Vector2f dir = Vector2f(std::cos(angle * to_rads), std::sin(angle * to_rads));
		if (get_component<RigidBody2D>()->velocity.x > 0)
		{
			dir.x *= -1;
		}

		get_component<RigidBody2D>()->velocity = dir * _speed;
		if (_bounce_paddle_sfx)
		{
			_audio_pool.play(_bounce_paddle_sfx.to_shared_ref());
		}
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
		if (_bounce_wall_sfx)
		{
			_audio_pool.play(_bounce_wall_sfx.to_shared_ref());
		}
	}
}
