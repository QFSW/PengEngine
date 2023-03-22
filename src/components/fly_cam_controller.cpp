#include "fly_cam_controller.h"

#include <input/input_subsystem.h>
#include <core/peng_engine.h>
#include <entities/camera.h>

IMPLEMENT_COMPONENT(components::FlyCamController);

using namespace components;
using namespace entities;
using namespace input;
using namespace math;

FlyCamController::FlyCamController()
	: _rot_sensitivity(0.1f)
	, _fly_speed(10)
{ }

void FlyCamController::post_create()
{
	Component::post_create();

	PengEngine::get().set_cursor_locked(true);
}

void FlyCamController::tick(float delta_time)
{
	Component::tick(delta_time);

	if (!Camera::current())
	{
		return;
	}

	const peng::shared_ref<Camera> camera = Camera::current().lock().to_shared_ref();
	Transform& camera_transform = camera->local_transform();

	const Vector3f fly_forwards = camera_transform.local_forwards();
	const Vector3f fly_right = camera_transform.local_right();
	const Vector3f fly_up = Vector3f::up();

	const Vector2i cursor_delta = InputSubsystem::get().cursor_delta();
	const Vector3f rot_input = Vector3i(cursor_delta.yx(), 0);

	Vector3f fly_input;
	fly_input += InputSubsystem::get()[KeyCode::d].is_down() ? +fly_right    : Vector3f::zero();
	fly_input += InputSubsystem::get()[KeyCode::a].is_down() ? -fly_right    : Vector3f::zero();
	fly_input += InputSubsystem::get()[KeyCode::w].is_down() ? +fly_forwards : Vector3f::zero();
	fly_input += InputSubsystem::get()[KeyCode::s].is_down() ? -fly_forwards : Vector3f::zero();
	fly_input += InputSubsystem::get()[KeyCode::e].is_down() ? +fly_up       : Vector3f::zero();
	fly_input += InputSubsystem::get()[KeyCode::q].is_down() ? -fly_up       : Vector3f::zero();

	camera_transform.position += fly_input.normalized() * _fly_speed * delta_time;
	camera_transform.rotation += rot_input * _rot_sensitivity;
	camera_transform.rotation.x = std::clamp<float>(camera_transform.rotation.x, -89.9f, 89.9f);
}
