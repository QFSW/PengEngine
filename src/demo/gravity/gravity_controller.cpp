#include "gravity_controller.h"

#include <profiling/scoped_event.h>
#include <components/fly_cam_controller.h>
#include <entities/camera.h>
#include <entities/point_light.h>
#include <math/math.h>

IMPLEMENT_ENTITY(demo::gravity::GravityController);

using namespace demo::gravity;
using namespace components;
using namespace entities;
using namespace math;

void GravityController::post_create()
{
	Entity::post_create();
	Logger::log("Gravity demo starting...");

	const peng::weak_ptr<Camera> camera = PengEngine::get().entity_manager().create_entity<Camera>();
	camera->make_perspective(70, 0.01f, 1000.0f);
	camera->local_transform().position = Vector3f(0, 0, -10);
	camera->add_component<FlyCamController>();

	for (int32_t i = 0; i < 500; i++)
	{
		auto rock = PengEngine::get().entity_manager().create_entity<Rock>(strtools::catf("Rock#%d", i));

		constexpr float field_radius = 10;
		constexpr float initial_speed = 2.5f;

		rock->mass = rand_range(0.001f, 0.05f);
		rock->local_transform().position = Vector3f(
			rand_range(-field_radius, field_radius), 
			rand_range(-field_radius, field_radius),
			rand_range(-field_radius, field_radius)
		);

		rock->velocity = Vector3f(
			rand_range(-initial_speed, initial_speed),
			rand_range(-initial_speed, initial_speed),
			rand_range(-initial_speed, initial_speed)
		);

		_rocks.push_back(rock);
	}

	peng::weak_ptr<PointLight> light = PengEngine::get().entity_manager().create_entity<PointLight>();
	light->data().range = 300;
	light->data().color = Vector3f(0.9f, 0.9f, 1);
	light->data().ambient = Vector3f(0.05f, 0.05f, 0);
	light->local_transform().position = Vector3f::up() * 20;
	
	Logger::success("Gravity demo started");
}

void GravityController::tick(float delta_time)
{
	SCOPED_EVENT("GravityController - tick");
	Entity::tick(delta_time);

	// Accessing via weak-ptr is quite slow due to thread safety so cache raw ptrs
	std::vector<Rock*> rock_ptrs;

	{
		SCOPED_EVENT("GravityController - cache pointers");

		rock_ptrs.reserve(_rocks.size());
		for (auto& rock : _rocks)
		{
			rock_ptrs.push_back(rock.lock().get());
		}
	}

	{
		SCOPED_EVENT("GravityController - apply attraction");

		for (size_t i = 0; i < _rocks.size(); i++)
		{
			Rock* rock1 = rock_ptrs[i];
			for (size_t j = 0; j < _rocks.size(); j++)
			{
				Rock* rock2 = rock_ptrs[j];
				if (i != j)
				{
					constexpr float gravity_strength = 10.0f;
					const Vector3f rock_1_to_2 = rock2->world_position() - rock1->world_position();
					const float dist_sqr = rock_1_to_2.magnitude_sqr();
					const float radii = rock1->radius() + rock2->radius();
					const float radius_sqr = radii * radii;

					if (dist_sqr > radius_sqr)
					{
						const Vector3f rock_1_to_2_dir = rock_1_to_2.normalized_unsafe();
						const float attraction = gravity_strength / rock_1_to_2.magnitude_sqr();

						rock1->velocity += rock_1_to_2_dir * attraction * rock2->mass * delta_time;
						rock2->velocity -= rock_1_to_2_dir * attraction * rock1->mass * delta_time;
					}
				}
			}
		}
	}
}
