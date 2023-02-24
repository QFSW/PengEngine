#include "rock.h"

#include <components/mesh_renderer.h>
#include <rendering/primitives.h>

IMPLEMENT_ENTITY(demo::gravity::Rock);

using namespace demo::gravity;
using namespace components;

void Rock::post_create()
{
	Entity::post_create();

	add_component<MeshRenderer>(
		rendering::Primitives::icosphere(1),
		copy_shared(rendering::Primitives::phong_material())
	);
}

void Rock::tick(float delta_time)
{
	Entity::tick(delta_time);

	_radius = std::powf(mass, 0.33f) * scale;

	_local_transform.scale = math::Vector3f::one() * _radius;
	_local_transform.position += velocity * delta_time;
}

float Rock::radius() const noexcept
{
	return _radius;
}
