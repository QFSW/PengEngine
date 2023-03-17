#include "paddle.h"

#include <components/mesh_renderer.h>
#include <rendering/primitives.h>

IMPLEMENT_ENTITY(demo::pong::Paddle);

using namespace demo::pong;
using namespace math;

Paddle::Paddle(const std::string& name)
	: Entity(name)
{
	add_component<components::MeshRenderer>(
		rendering::Primitives::quad(),
		rendering::Primitives::unlit_material()
	);

	_local_transform.scale = Vector3f(1, 7, 1);
}

void Paddle::tick(float delta_time)
{
	Entity::tick(delta_time);

	const float up_amount = PengEngine::get().input_manager().axis_value(input_axis);
	_local_transform.position += Vector3f::up() * up_amount * movement_speed * delta_time;
}
