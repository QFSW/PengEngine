#include "peng_pong.h"

#include <core/asset.h>
#include <core/serialized_member.h>
#include <core/logger.h>
#include <core/peng_engine.h>
#include <audio/audio_clip.h>
#include <profiling/scoped_event.h>
#include <entities/camera.h>
#include <components/sprite_renderer.h>
#include <components/text_renderer.h>
#include <components/box_collider_2d.h>
#include <rendering/window_subsystem.h>
#include <input/input_subsystem.h>

#include "ball.h"
#include "goal.h"
#include "paddle.h"
#include "pause_menu.h"

IMPLEMENT_ENTITY(demo::pong::PengPong);

using namespace demo::pong;
using namespace rendering;
using namespace entities;
using namespace components;
using namespace input;
using namespace math;

PengPong::PengPong()
    : Entity("PengPong")
{
	SERIALIZED_MEMBER(_menu_select_sfx);
	SERIALIZED_MEMBER(_menu_click_sfx);
}

void PengPong::post_create()
{
	Entity::post_create();
	Logger::log("PengPong starting...");

	srand(static_cast<unsigned int>(time(nullptr)));

	PengEngine::get().set_max_delta_time(50.0);
	WindowSubsystem::get().set_window_name("Peng Pong");

	build_camera();
	build_main_menu();

	Logger::success("PengPong started");
}

void PengPong::tick(float delta_time)
{
    Entity::tick(delta_time);

	if (_game_state == GameState::main_menu)
	{
	    if (InputSubsystem::get()[KeyCode::enter].pressed())
	    {
			_audio_pool.play(_menu_click_sfx.to_shared_ref());

			_menu_root->set_active(false);
			_game_state = GameState::playing;
			build_world();
	    }
	}

	if (InputSubsystem::get()[KeyCode::escape].pressed())
	{
		if (_game_state == GameState::playing)
		{
			pause();
		}
		else if (_game_state == GameState::paused)
		{
			unpause();
		}
	    else if (_game_state == GameState::main_menu)
		{
			quit();
		}
	}
}

void PengPong::build_camera()
{
	peng::weak_ptr<Camera> camera = create_entity<Camera>();
	camera->make_orthographic(ortho_size, 0.01f, 100.0f);
}

void PengPong::build_main_menu()
{
	_menu_root = create_entity<Entity>("MainMenu");
	_menu_root->local_transform().position = Vector3f(0, 0, 5);

	peng::weak_ptr<Entity> text = _menu_root->create_child<Entity>("Text");
	text->local_transform().scale = Vector3f::one() * digit_size;
	text->add_component<TextRenderer>()->set_text("Peng Pong");

	peng::weak_ptr<Entity> play_text = _menu_root->create_child<Entity>("RestartText");
	play_text->local_transform().position = Vector3f(0, -5, 0);
	play_text->local_transform().scale = Vector3f::one() * digit_size / 4;
	play_text->add_component<TextRenderer>()->set_text("Press Enter to play");

	peng::weak_ptr<Entity> credits = _menu_root->create_child<Entity>("Credits");
	credits->local_transform().position = Vector3f(0, -ortho_size + 1, 0);
	credits->local_transform().scale = Vector3f::one() * digit_size / 6;
	credits->add_component<TextRenderer>()->set_text("Made with Peng Engine by QFSW");
}

void PengPong::build_world()
{
	SCOPED_EVENT("PengPong - build world");

	if (_world_root)
	{
		_world_root->destroy();
	}

	_world_root = create_entity<Entity>("World");

	const float ortho_width = ortho_size * WindowSubsystem::get().aspect_ratio();
	const float paddle_delta_x = ortho_width - paddle_margin;

	peng::weak_ptr<Ball> ball = _world_root->create_child<Ball>();

	peng::weak_ptr<Paddle> paddle_1 = _world_root->create_child<Paddle>("Paddle1");
	paddle_1->input_axis.positive = KeyCode::w;
	paddle_1->input_axis.negative = KeyCode::s;
	paddle_1->local_transform().position = Vector3f(-paddle_delta_x, 0, 0);

	peng::weak_ptr<Paddle> paddle_2 = _world_root->create_child<Paddle>("Paddle2");
	paddle_2->input_axis.positive = KeyCode::up;
	paddle_2->input_axis.negative = KeyCode::down;
	paddle_2->local_transform().position = Vector3f(+paddle_delta_x, 0, 0);

	peng::weak_ptr<Entity> score_1 = _world_root->create_child<Entity>("Score1");
	peng::weak_ptr<TextRenderer> score_1_text = score_1->add_component<TextRenderer>();
	score_1->local_transform().scale = Vector3f::one() * digit_size;
	score_1->local_transform().position = Vector3f(-digit_size * 2, ortho_size * 0.75f, 5);
	score_1_text->set_text("0");

	peng::weak_ptr<Entity> score_2 = _world_root->create_child<Entity>("Score2");
	peng::weak_ptr<TextRenderer> score_2_text = score_2->add_component<TextRenderer>();
	score_2->local_transform().scale = Vector3f::one() * 5;
	score_2->local_transform().position = Vector3f(digit_size * 2, ortho_size * 0.75f, 5);
	score_2_text->set_text("0");

	// TODO: should use a subscribe_weak for better safety
	paddle_1->on_score_changed().subscribe([score_1_text](int32_t score)
		{
			score_1_text->set_text(strtools::catf("%d", score));
		});

	paddle_2->on_score_changed().subscribe([score_2_text](int32_t score)
		{
			score_2_text->set_text(strtools::catf("%d", score));
		});

	peng::weak_ptr<Entity> barrier_top = _world_root->create_child<Entity>("BarrierTop");
	barrier_top->add_component<components::BoxCollider2D>();
	barrier_top->local_transform().position = Vector3f(0, ortho_size + 2, 0);
	barrier_top->local_transform().scale = Vector3f(ortho_width * 3, 3, 1);

	peng::weak_ptr<Entity> barrier_bottom = _world_root->create_child<Entity>("BarrierBottom");
	barrier_bottom->add_component<components::BoxCollider2D>();
	barrier_bottom->local_transform().position = Vector3f(0, -ortho_size - 2, 0);
	barrier_bottom->local_transform().scale = Vector3f(ortho_width * 3, 3, 1);

	peng::weak_ptr<Goal> goal_right = _world_root->create_child<Goal>(paddle_1);
	goal_right->local_transform().position = Vector3f(ortho_width + 2, 0, 0);
	goal_right->local_transform().scale = Vector3f(2, ortho_size * 3, 1);

	peng::weak_ptr<Goal> goal_left = _world_root->create_child<Goal>(paddle_2);
	goal_left->local_transform().position = Vector3f(-ortho_width - 2, 0, 0);
	goal_left->local_transform().scale = Vector3f(2, ortho_size * 3, 1);

	peng::weak_ptr<Entity> background = _world_root->create_child<Entity>("Background");

	const Vector3f stripe_size = Vector3f(0.5f, 1.5f, 1);
	const float stripe_padding = 1.5f;
	const float stripe_spacing = stripe_size.y + stripe_padding;
	const int32_t num_stripes = static_cast<int32_t>(ortho_size / stripe_spacing);

	for (int i = -num_stripes; i <= num_stripes; i++)
	{
		peng::weak_ptr<Entity> stripe = background->create_child<Entity, EntityRelationship::activity>("Stripe");
		stripe->local_transform().scale = stripe_size;
		stripe->local_transform().position = Vector3f(0, i * stripe_spacing, 0);
		stripe->add_component<SpriteRenderer>();
	}
}

void PengPong::pause()
{
	_game_state = GameState::paused;
	PengEngine::get().set_time_scale(0);

	if (_pause_root)
	{
		_pause_root->show();
	}
	else
	{
		_pause_root = create_entity<PauseMenu>();
		_pause_root->pause_size = digit_size;
		_pause_root->btn_size = digit_size / 4;

		_pause_root->on_selection_change().subscribe([weak_this = weak_this()]
		{
			if (weak_this->_menu_select_sfx)
			{
				weak_this->_audio_pool.play(weak_this->_menu_select_sfx.to_shared_ref());
			}
		});

		_pause_root->on_selection_click().subscribe([weak_this = weak_this()]
		{
			if (weak_this->_menu_click_sfx)
			{
				weak_this->_audio_pool.play(weak_this->_menu_click_sfx.to_shared_ref());
			}
		});

		_pause_root->on_resume().subscribe([weak_this = weak_this()]
		{
			weak_this->unpause();
		});

		_pause_root->on_restart().subscribe([weak_this = weak_this()]
		{
			weak_this->build_world();
			weak_this->unpause();
		});

		_pause_root->on_quit().subscribe([weak_this = weak_this()]
		{
			weak_this->quit();
		});
	}
}

void PengPong::unpause()
{
	_game_state = GameState::playing;
	PengEngine::get().set_time_scale(1);

	if (_pause_root)
	{
		_pause_root->hide();
	}
}

void PengPong::quit()
{
	PengEngine::get().request_shutdown();
}
