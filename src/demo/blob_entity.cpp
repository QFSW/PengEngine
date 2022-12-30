#include "blob_entity.h"

#include <entities/camera.h>

using namespace math;
using namespace components;

BlobEntity::BlobEntity(
	const peng::shared_ref<const rendering::Mesh>& mesh,
	const peng::shared_ref<rendering::Material>& material,
	const Vector2f& pos
)
	: Entity(true)
	, _age(static_cast<float>(rand()) / static_cast<float>((RAND_MAX)))
{
	_transform = Transform(Vector3f(pos, pos.y), Vector3f::one(), Vector3f::zero());
	_mesh_renderer = add_component<MeshRenderer>(mesh, material);
}

void BlobEntity::tick(double delta_time)
{
	Entity::tick(delta_time);

	_age += static_cast<float>(delta_time);
	_mesh_renderer->material()->set_parameter("time", _age);
}