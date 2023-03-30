#include "gravity_controller.h"

#include <algorithm>
#include <execution>

#include <core/peng_engine.h>
#include <profiling/scoped_event.h>
#include <entities/directional_light.h>
#include <entities/skybox.h>
#include <rendering/texture.h>
#include <rendering/material.h>
#include <rendering/primitives.h>
#include <math/math.h>

IMPLEMENT_ENTITY(demo::gravity::GravityController);

using namespace demo::gravity;
using namespace entities;
using namespace rendering;
using namespace math;

void GravityController::post_create()
{
	Entity::post_create();
	Logger::log("Gravity demo starting...");

	PengEngine::get().set_max_delta_time(50.0);
	PengEngine::get().set_window_name("PengEngine - Gravity Demo");

	create_rock_field(500, 5, 2);
	create_rock_field(100, 10, 4);

	peng::weak_ptr<DirectionalLight> light = create_entity<DirectionalLight>();
	light->data().color = Vector3f(1, 1, 0.9f);
	light->data().ambient = Vector3f(0.1f, 0.1f, 0.15f);
	light->local_transform().rotation = Vector3f(20, 20, 0);

	peng::shared_ref<const Texture> skybox_texture = peng::make_shared<Texture>("skybox",
		"resources/textures/demo/skybox.jpg"
	);

	peng::weak_ptr<Skybox> skybox = create_entity<Skybox>();
	skybox->material()->set_parameter("color_tex", skybox_texture);
	skybox->set_parent(weak_this());
	
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

		std::for_each(
			std::execution::par_unseq, rock_ptrs.begin(), rock_ptrs.end(),
			[&](Rock* rock1) {
				for (Rock* rock2 : rock_ptrs)
				{
					if (rock1 != rock2)
					{
						constexpr float gravity_strength = 20.0f;
						const Vector3f rock_1_to_2 = rock2->world_position() - rock1->world_position();
						const float dist_sqr = rock_1_to_2.magnitude_sqr();
						const float radii = rock1->radius() + rock2->radius();
						const float radius_sqr = radii * radii;

						if (dist_sqr > radius_sqr)
						{
							const Vector3f rock_1_to_2_dir = rock_1_to_2.normalized_unsafe();
							const float attraction = gravity_strength / rock_1_to_2.magnitude_sqr();
							rock1->velocity += rock_1_to_2_dir * attraction * rock2->mass * delta_time;
						}
					}
				}
			});
	}
}

void GravityController::create_rock_field(int32_t count, float radius, float speed)
{
	SCOPED_EVENT("GravityController - create rock field", strtools::catf_temp("%d rocks", count));
	for (int32_t i = 0; i < count; i++)
	{
		auto rock = create_entity<Rock>(strtools::catf("Rock#%d", i));

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
