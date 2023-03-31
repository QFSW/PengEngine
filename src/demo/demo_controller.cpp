#include "demo_controller.h"

#include <core/asset.h>
#include <rendering/primitives.h>
#include <rendering/material.h>
#include <input/input_subsystem.h>
#include <entities/point_light.h>
#include <entities/skybox.h>
#include <components/fly_cam_controller.h>
#include <components/mesh_renderer.h>

#include "blob_entity.h"
#include "rendering/utils.h"

IMPLEMENT_ENTITY(demo::DemoController);

using namespace demo;
using namespace rendering;
using namespace math;
using namespace entities;
using namespace input;

DemoController::DemoController()
	: Entity("DemoController")
{ }

void DemoController::post_create()
{
	Entity::post_create();

	Logger::log("Demo controller starting...");

	const auto blobs_entity = create_entity<Entity>("Blobs", TickGroup::none);

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

			blobs_entity->create_child<BlobEntity>(mesh, pos);
		}
	}

	peng::shared_ref<const Texture> skybox_texture = peng::make_shared<Texture>("skybox",
		"resources/textures/demo/skybox.jpg"
	);

	peng::weak_ptr<Skybox> skybox = create_entity<Skybox>();
	skybox->material()->set_parameter("color_tex", skybox_texture);
	skybox->set_parent(weak_this());

	const Vector2f floor_size(500, 500);
	const auto floor_material = Primitives::phong_material();
	floor_material->set_parameter("color_tex", Asset<Texture>("resources/textures/demo/wall.asset").load_const());
	floor_material->set_parameter("base_color", Vector3f(0.7f, 1, 0.7f));
	floor_material->set_parameter("tex_scale", floor_size);
	floor_material->set_parameter<float>("shinyness", 8);

	const auto floor_entity = create_entity<Entity>("Floor", TickGroup::none);
	const auto floor_renderer = floor_entity->add_component<components::MeshRenderer>(Primitives::fullscreen_quad(), floor_material);
	floor_entity->local_transform() = Transform(
		Vector3f(0, -5, 0),
		Vector3f(floor_size, 1),
		Vector3f(-90, 0, 0)
	);

	for (int32_t i = 0; i < 4; i++)
	{
		_light_entities.push_back(create_entity<PointLight>());
		_light_entities[i]->data().range = 100;
		_light_entities[i]->local_transform().position = Vector3f(i * 5.0f, 0, 0);
		_light_renderers.push_back(_light_entities[i]->add_component<components::MeshRenderer>(Primitives::icosphere(4), Primitives::unlit_material()));
	}

	Logger::success("Demo controller started");
}

void DemoController::tick(float delta_time)
{
	Entity::tick(delta_time);
	
	_age += delta_time;

	if (InputSubsystem::get()[KeyCode::num_row_9].pressed())
	{
		EntitySubsystem::get().find_entity("Blob", true)->destroy();
	}

	Vector3f light_delta = Vector3f::zero();
	light_delta += InputSubsystem::get()[KeyCode::up].is_down() ? +Vector3f::forwards() : Vector3f::zero();
	light_delta += InputSubsystem::get()[KeyCode::down].is_down() ? -Vector3f::forwards() : Vector3f::zero();
	light_delta += InputSubsystem::get()[KeyCode::right].is_down() ? +Vector3f::right() : Vector3f::zero();
	light_delta += InputSubsystem::get()[KeyCode::left].is_down() ? -Vector3f::right() : Vector3f::zero();
	light_delta += InputSubsystem::get()[KeyCode::n].is_down() ? +Vector3f::up() : Vector3f::zero();
	light_delta += InputSubsystem::get()[KeyCode::m].is_down() ? -Vector3f::up() : Vector3f::zero();

	float light_range_delta = 0;
	light_range_delta += InputSubsystem::get()[KeyCode::t].is_down() ? +1 : 0;
	light_range_delta += InputSubsystem::get()[KeyCode::y].is_down() ? -1 : 0;

	for (size_t i = 0; i < _light_entities.size(); i++)
	{
		if (_light_entities[i])
		{
			PointLight::LightData& light_data = _light_entities[i]->data();

			_light_entities[i]->local_transform().position += light_delta * 5 * delta_time;
			light_data.range *= (1 + light_range_delta * delta_time);

			const float age = _age * (1 + 0.3f * i);
			light_data.color = Vector3f::one() * 0.5 + Vector3f(std::sin(age), std::sin(age * 1.2f), std::sin(age * 1.4f)) / 2;

			_light_entities[i]->local_transform().scale = Vector3f::one() * 0.2f * std::powf(light_data.range, 0.33f);
			_light_renderers[i]->material()->set_parameter("base_color", light_data.color);
		}
	}
}
