#include "demo_controller.h"

#include <core/peng_engine.h>
#include <core/logger.h>
#include <rendering/primitives.h>
#include <rendering/material.h>
#include <entities/camera.h>
#include <components/fly_cam_controller.h>
#include <input/input_manager.h>

#include "blob_entity.h"

using namespace demo;
using namespace rendering;
using namespace math;
using namespace entities;

DemoController::DemoController()
	: Entity("DemoController")
{ }

void DemoController::post_create()
{
	Entity::post_create();

	Logger::log("Demo controller starting...");

	const peng::weak_ptr<Camera> camera = PengEngine::get().entity_manager().create_entity<Camera>();
	camera->make_perspective(70, 0.01f, 1000.0f);
	camera->local_transform().position = Vector3f(0, 0, -10);
	camera->add_component<components::FlyCamController>();

	const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
		"Rave",
		"resources/shaders/demo/wobble.vert",
		"resources/shaders/demo/rave.frag"
	);

	const peng::shared_ref<const Texture> texture = peng::make_shared<Texture>("wall",
		"resources/textures/demo/wall.jpg"
	);

	auto material = peng::make_shared<Material>(shader);
	material->set_parameter("color_tex", texture);

	const auto blobs_entity = PengEngine::get().entity_manager().create_entity<Entity>("Blobs", TickGroup::none);

	const Vector2i blob_grid(7, 5);
	for (int32_t blob_x = 0; blob_x < blob_grid.x; blob_x++)
	{
		for (int32_t blob_y = 0; blob_y < blob_grid.y; blob_y++)
		{
			constexpr float blob_spacing = 1.75f;
			const Vector2f pos = Vector2f(blob_x - (blob_grid.x - 1) / 2.0f, blob_y - (blob_grid.y - 1) / 2.0f)
				* blob_spacing;

			auto mesh = blob_x % 2
				? Primitives::cube()
				: Primitives::icosphere(blob_y);

			PengEngine::get().entity_manager().create_entity<BlobEntity>(mesh, material, pos)
				->set_parent(blobs_entity);
		}
	}

	const Vector2f floor_size(500, 500);
	const auto floor_material = copy_shared(Primitives::phong_material());
	floor_material->set_parameter("color_tex", texture);
	floor_material->set_parameter("base_color", Vector4f(0, 1, 0, 1));
	floor_material->set_parameter("tex_scale", floor_size);

	const auto floor_entity = PengEngine::get().entity_manager().create_entity<Entity>("Floor", TickGroup::none);
	const auto floor_renderer = floor_entity->add_component<components::MeshRenderer>(Primitives::fullscreen_quad(), floor_material);
	floor_entity->local_transform() = Transform(
		Vector3f(0, -5, 0),
		Vector3f(floor_size, 1),
		Vector3f(-90, 0, 0)
	);

	const auto light_entity = PengEngine::get().entity_manager().create_entity<Entity>("Light", TickGroup::none);
	light_entity->add_component<components::MeshRenderer>(Primitives::icosphere(4), peng::copy_shared(Primitives::unlit_material()));

	Logger::success("Demo controller started");
}

void DemoController::tick(float delta_time)
{
	Entity::tick(delta_time);

	using namespace input;
	const InputManager& input_manager = PengEngine::get().input_manager();

	if (input_manager[KeyCode::num_row_0].pressed())
	{
		PengEngine::get().entity_manager().dump_hierarchy();
	}

	if (input_manager[KeyCode::num_row_9].pressed())
	{
		PengEngine::get().entity_manager().destroy_entity(PengEngine::get().entity_manager().find_entity("Blob", true));
	}

	if (input_manager[KeyCode::f11].pressed())
	{
		PengEngine::get().maximize_window();
	}

	if (input_manager[KeyCode::escape].pressed())
	{
		PengEngine::get().request_shutdown();
	}
}
