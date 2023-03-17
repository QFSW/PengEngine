#include "paddle.h"

#include <components/mesh_renderer.h>
#include <components/rigid_body_2d.h>
#include <components/box_collider_2d.h>
#include <rendering/primitives.h>

IMPLEMENT_ENTITY(demo::pong::Paddle);

using namespace demo::pong;
using namespace components;
using namespace math;

Paddle::Paddle(const std::string& name)
	: Entity(name)
{
	add_component<RigidBody2D>();
	add_component<BoxCollider2D>();
	add_component<MeshRenderer>(
		rendering::Primitives::quad(),
		rendering::Primitives::unlit_material()
	);

	_local_transform.scale = Vector3f(1, 7, 1);
}

void Paddle::tick(float delta_time)
{
	Entity::tick(delta_time);

	const float up_amount = PengEngine::get().input_manager().axis_value(input_axis);
	get_component<RigidBody2D>()->velocity = Vector2f::up() * up_amount * movement_speed;
}
