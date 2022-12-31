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
	, _rot_speed(45)
	, _zoom_speed(1)
{ }

void DemoController::post_create()
{
	Entity::post_create();

	Logger::get().log(LogSeverity::log, "Demo controller starting...");

	const peng::weak_ptr<Camera> camera = PengEngine::get().entity_manager().create_entity<Camera>();
	camera->make_perspective(70, 0.0001f, 100.0f);
	camera->transform().position = Vector3f(0, 0, -10);

	const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
		"Rave",
		"resources/shaders/core/projection.vert",
		"resources/shaders/demo/rave.frag"
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

void DemoController::tick(float delta_time)
{
	Entity::tick(delta_time);

	if (!Camera::current())
	{
		return;
	}

	using namespace input;
	const InputManager& input = PengEngine::get().input_manager();

	Vector3f pan_input;
	Vector3f rot_input;
	float zoom_input = 0;

	if (input[KeyCode::a].is_down())
	{
		pan_input.x -= 1;
	}

	if (input[KeyCode::d].is_down())
	{
		pan_input.x += 1;
	}

	if (input[KeyCode::s].is_down())
	{
		pan_input.y -= 1;
	}

	if (input[KeyCode::w].is_down())
	{
		pan_input.y += 1;
	}

	if (input[KeyCode::left].is_down())
	{
		rot_input.y -= 1;
	}

	if (input[KeyCode::right].is_down())
	{
		rot_input.y += 1;
	}

	if (input[KeyCode::down].is_down())
	{
		rot_input.x += 1;
	}

	if (input[KeyCode::up].is_down())
	{
		rot_input.x -= 1;
	}

	if (input[KeyCode::q].is_down() || input[KeyCode::z].is_down())
	{
		zoom_input -= 1;
	}

	if (input[KeyCode::e].is_down() || input[KeyCode::x].is_down())
	{
		zoom_input += 1;
	}

	const peng::shared_ref<Camera> camera = Camera::current().lock().to_shared_ref();
	const Camera::Projection projection = camera->projection();
	const bool orthographic = projection == Camera::Projection::orthographic;

	if (orthographic)
	{
		const float zoom_amount = _zoom_speed * static_cast<float>(delta_time);
		Camera::current()->ortho_size() /= (1 + zoom_input * zoom_amount);
	}
	else
	{
		pan_input = Vector3f(pan_input.x, zoom_input, pan_input.y);
	}

	const float pan_amount = _pan_speed * static_cast<float>(delta_time)
		* (orthographic ? Camera::current()->ortho_size() : 1);

	const Vector3f camera_rotation = camera->transform().rotation;
	const Matrix4x4f rotator = Matrix4x4f::identity().rotated(camera_rotation);

	// TODO: something about rotation and camera is completely broken
	camera->transform().position += (rotator * pan_input.normalized()) * pan_amount;
	camera->transform().rotation += rot_input.normalized() * pan_amount * _rot_speed / _pan_speed;
}
