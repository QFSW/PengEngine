#include "blob_entity.h"

#include <entities/camera.h>
#include <core/peng_engine.h>
#include <rendering/primitives.h>

using namespace math;
using namespace components;
using namespace rendering;

BlobEntity::BlobEntity(
	const peng::shared_ref<const Mesh>& mesh,
	const peng::shared_ref<Material>& material,
	const Vector2f& pos
)
	: Entity("Blob")
	, _age(static_cast<float>(rand()) / static_cast<float>((RAND_MAX)))
{
	_local_transform = Transform(Vector3f(pos, pos.y + 2), Vector3f::one(), Vector3f::zero());
	_mesh_renderer = add_component<MeshRenderer>(mesh, material);
}

void BlobEntity::post_create()
{
	Entity::post_create();

	const std::vector<Vector3f> child_positions = {
		Vector3f(0.5f, 0, 0),
		Vector3f(0, 0.5f, 0),
		Vector3f(0, 0, 0.5f)
	};

	for (const Vector3f& child_position : child_positions)
	{
		const peng::shared_ref<Material> child_material = peng::copy_shared(Primitives::phong_material());
		child_material->set_parameter("base_color", child_position * 2);

		const peng::weak_ptr<Entity> entity = PengEngine::get().entity_manager().create_entity<Entity>("AxisMarker");
		entity->add_component<MeshRenderer>(Primitives::cube(), child_material);
		entity->local_transform().scale = Vector3f::one() / 4;
		entity->local_transform().position = child_position;
		entity->set_parent(weak_this());
	}
}

void BlobEntity::tick(float delta_time)
{
	Entity::tick(delta_time);

	Vector3f rotation;
	if (PengEngine::get().input_manager()[input::KeyCode::u].is_down()) { rotation.x += 1; }
	if (PengEngine::get().input_manager()[input::KeyCode::j].is_down()) { rotation.x -= 1; }
	if (PengEngine::get().input_manager()[input::KeyCode::i].is_down()) { rotation.y += 1; }
	if (PengEngine::get().input_manager()[input::KeyCode::k].is_down()) { rotation.y -= 1; }
	if (PengEngine::get().input_manager()[input::KeyCode::o].is_down()) { rotation.z += 1; }
	if (PengEngine::get().input_manager()[input::KeyCode::l].is_down()) { rotation.z -= 1; }

	_local_transform.rotation += rotation * 90 * delta_time;

	_age += delta_time;
	_mesh_renderer->material()->set_parameter("time", _age);
}
