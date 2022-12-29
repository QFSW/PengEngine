#include "demo_controller.h"

#include <core/peng_engine.h>
#include <core/logger.h>
#include <rendering/primitives.h>
#include <rendering/material.h>
#include <entities/camera.h>
#include <input/input_manager.h>

#include "blob_entity.h"

using namespace demo;
using namespace rendering;
using namespace math;
using namespace entities;

DemoController::DemoController()
	: Entity(true)
	, _pan_speed(5)
{ }

void DemoController::post_create()
{
	Entity::post_create();

	Logger::get().log(LogSeverity::log, "Demo controller starting...");
	PengEngine::get().entity_manager().create_entity<Camera>(7.0f, 0.0f, 10000.0f);

	const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
		"resources/shaders/core/projection.vert",
		"resources/shaders/core/unlit.frag"
	);

	const peng::shared_ref<const Texture> texture = peng::make_shared<Texture>("wall",
		"resources/textures/demo/wall.jpg"
	);

	auto material = peng::make_shared<Material>(shader);
	material->set_parameter("color_tex", texture);

	const Vector2i blob_grid(7, 4);
	for (int32_t blob_x = 0; blob_x < blob_grid.x; blob_x++)
	{
		for (int32_t blob_y = 0; blob_y < blob_grid.y; blob_y++)
		{
			constexpr float blob_spacing = 1.75f;
			const Vector2f pos = Vector2f(blob_x - (blob_grid.x - 1) / 2.0f, blob_y - (blob_grid.y - 1) / 2.0f)
				* blob_spacing;
			PengEngine::get().entity_manager().create_entity<BlobEntity>(Primitives::cube(), material, pos);
		}
	}

	Logger::get().log(LogSeverity::success, "Demo controller started");
}

void DemoController::tick(double delta_time)
{
	Entity::tick(delta_time);

	if (!Camera::current())
	{
		return;
	}

	using namespace input;
	const InputManager& input = PengEngine::get().input_manager();

	Vector2f input_vector;

	if (input[KeyCode::left].is_down())
	{
		input_vector.x -= 1;
	}

	if (input[KeyCode::right].is_down())
	{
		input_vector.x += 1;
	}

	if (input[KeyCode::down].is_down())
	{
		input_vector.y -= 1;
	}

	if (input[KeyCode::up].is_down())
	{
		input_vector.y += 1;
	}

	const float dist = _pan_speed * static_cast<float>(delta_time);
	Camera::current()->transform().position += Vector3f(input_vector.normalized(), 0) * dist;
}
