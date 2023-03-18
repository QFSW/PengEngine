#include "peng_pong.h"

#include <core/logger.h>
#include <profiling/scoped_event.h>
#include <entities/camera.h>
#include <components/box_collider_2d.h>

#include "goal.h"
#include "paddle.h"

IMPLEMENT_ENTITY(demo::pong::PengPong);

using namespace demo::pong;
using namespace entities;
using namespace math;

void PengPong::post_create()
{
	Entity::post_create();
	Logger::log("PengPong starting...");

	srand(static_cast<unsigned int>(time(nullptr)));

	PengEngine::get().set_max_delta_time(50.0);
	PengEngine::get().set_window_name("PengPong");

	constexpr float ortho_size = 20;
	constexpr float paddle_margin = 3;

	const float ortho_width = ortho_size * PengEngine::get().aspect_ratio();
	const float paddle_delta_x = ortho_width - paddle_margin;

	// TODO: PengEngine::get().entity_manager().create_entity is far too long
	peng::weak_ptr<Camera> camera = PengEngine::get().entity_manager().create_entity<Camera>();
	camera->make_orthographic(ortho_size, 0.01f, 2.0f);

	peng::weak_ptr<Paddle> paddle_1 = PengEngine::get().entity_manager().create_entity<Paddle>("Paddle1");
	paddle_1->input_axis.positive = input::KeyCode::w;
	paddle_1->input_axis.negative = input::KeyCode::s;
	paddle_1->local_transform().position = Vector3f(-paddle_delta_x, 0, 0);

	peng::weak_ptr<Paddle> paddle_2 = PengEngine::get().entity_manager().create_entity<Paddle>("Paddle2");
	paddle_2->input_axis.positive = input::KeyCode::up;
	paddle_2->input_axis.negative = input::KeyCode::down;
	paddle_2->local_transform().position = Vector3f(+paddle_delta_x, 0, 0);

	peng::weak_ptr<Entity> barrier_top = PengEngine::get().entity_manager().create_entity<Entity>("BarrierTop");
	barrier_top->add_component<components::BoxCollider2D>();
	barrier_top->local_transform().position = Vector3f(0, ortho_size + 0.5f, 0);
	barrier_top->local_transform().scale = Vector3f(ortho_width * 3, 1, 1);

	peng::weak_ptr<Entity> barrier_bottom = PengEngine::get().entity_manager().create_entity<Entity>("BarrierBottom");
	barrier_bottom->add_component<components::BoxCollider2D>();
	barrier_bottom->local_transform().position = Vector3f(0, -ortho_size - 0.5f, 0);
	barrier_bottom->local_transform().scale = Vector3f(ortho_width * 3, 1, 1);

	peng::weak_ptr<Goal> goal_right = PengEngine::get().entity_manager().create_entity<Goal>(paddle_1);
	goal_right->local_transform().position = Vector3f(ortho_width + 2, 0, 0);
	goal_right->local_transform().scale = Vector3f(2, ortho_size * 3, 1);

	peng::weak_ptr<Goal> goal_left = PengEngine::get().entity_manager().create_entity<Goal>(paddle_2);
	goal_left->local_transform().position = Vector3f(-ortho_width - 2, 0, 0);
	goal_left->local_transform().scale = Vector3f(2, ortho_size * 3, 1);

	Logger::success("PengPong started");
}

void PengPong::tick(float delta_time)
{
	SCOPED_EVENT("PengPong - tick");
	Entity::tick(delta_time);
}
