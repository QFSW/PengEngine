#include "gravity_controller.h"

#include <profiling/scoped_event.h>
#include <entities/point_light.h>
#include <math/math.h>

IMPLEMENT_ENTITY(demo::gravity::GravityController);

using namespace demo::gravity;
using namespace entities;
using namespace math;

void GravityController::post_create()
{
	Entity::post_create();
	Logger::log("Gravity demo starting...");

	PengEngine::get().set_max_delta_time(50.0);
	PengEngine::get().set_window_name("PengEngine - Gravity Demo");

	create_rock_field(500, 5, 2);
	create_rock_field(100, 10, 4);

	peng::weak_ptr<PointLight> light = PengEngine::get().entity_manager().create_entity<PointLight>();
	light->data().range = 1000;
	light->data().color = Vector3f(0.9f, 0.9f, 1);
	light->data().ambient = Vector3f(0.05f, 0.05f, 0);
	light->local_transform().position = Vector3f::up() * 30;
	
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

						const Vector3f dv = rock_1_to_2_dir * attraction * delta_time;
						rock1->velocity += dv * rock2->mass;
						rock2->velocity -= dv * rock1->mass;
					}
				}
			}
		}
	}
}

void GravityController::create_rock_field(int32_t count, float radius, float speed)
{
	SCOPED_EVENT("GravityController - create rock field", strtools::catf_temp("%d rocks", count));
	for (int32_t i = 0; i < count; i++)
	{
		auto rock = PengEngine::get().entity_manager().create_entity<Rock>(strtools::catf("Rock#%d", i));

		rock->mass = rand_range(0.001f, 0.05f);
		rock->local_transform().position = Vector3f(
			rand_range(-radius, radius),
			rand_range(-radius, radius),
			rand_range(-radius, radius)
		);

		rock->velocity = Vector3f(
			rand_range(-speed, speed),
			rand_range(-speed, speed),
			rand_range(-speed, speed)
		);

		_rocks.push_back(rock);
	}
}
