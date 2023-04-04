#include "blob_entity.h"

#include <core/asset.h>
#include <entities/camera.h>
#include <input/input_subsystem.h>
#include <rendering/material.h>
#include <rendering/primitives.h>

using namespace math;
using namespace components;
using namespace rendering;
using namespace input;

IMPLEMENT_ENTITY(BlobEntity);

BlobEntity::BlobEntity(
	const peng::shared_ref<const Mesh>& mesh,
	const Vector2f& pos
)
	: Entity("Blob")
	, _age(static_cast<float>(rand()) / static_cast<float>((RAND_MAX)))
{
	Asset<Shader> blob_shader("resources/shaders/demo/blob.asset");
	Asset<Texture> wall_texture("resources/textures/demo/wall.asset");

	peng::shared_ref<const Shader> shader = blob_shader.load();
	peng::shared_ref<Material> material = peng::make_shared<Material>(shader);
	material->set_parameter("color_tex", wall_texture.load());

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
		const peng::shared_ref<Material> child_material = Primitives::phong_material();
		child_material->set_parameter("base_color", Vector4f(child_position * 2, 1));

		const peng::weak_ptr<Entity> entity = create_child<Entity>("AxisMarker");
		entity->add_component<MeshRenderer>(Primitives::cube(), child_material);
		entity->local_transform().scale = Vector3f::one() / 4;
		entity->local_transform().position = child_position;
	}
}

void BlobEntity::tick(float delta_time)
{
	Entity::tick(delta_time);

	Vector3f rotation;
	if (InputSubsystem::get()[KeyCode::u].is_down()) { rotation.x += 1; }
	if (InputSubsystem::get()[KeyCode::j].is_down()) { rotation.x -= 1; }
	if (InputSubsystem::get()[KeyCode::i].is_down()) { rotation.y += 1; }
	if (InputSubsystem::get()[KeyCode::k].is_down()) { rotation.y -= 1; }
	if (InputSubsystem::get()[KeyCode::o].is_down()) { rotation.z += 1; }
	if (InputSubsystem::get()[KeyCode::l].is_down()) { rotation.z -= 1; }

	_local_transform.rotation += rotation * 90 * delta_time;

	_age += delta_time;
	_mesh_renderer->material()->set_parameter("time", _age);
}
