#include "peng_pong.h"

#include <core/logger.h>
#include <profiling/scoped_event.h>
#include <entities/camera.h>
#include <components/sprite_renderer.h>
#include <components/box_collider_2d.h>
#include <rendering/texture.h>
#include <rendering/sprite.h>

#include "goal.h"
#include "paddle.h"
#include "number_display.h"

IMPLEMENT_ENTITY(demo::pong::PengPong);

using namespace demo::pong;
using namespace rendering;
using namespace entities;
using namespace math;

void PengPong::post_create()
{
	Entity::post_create();
	Logger::log("PengPong starting...");

	srand(static_cast<unsigned int>(time(nullptr)));

	PengEngine::get().set_max_delta_time(50.0);
	PengEngine::get().set_window_name("PengPong");

	build_world();

	Logger::success("PengPong started");
}

void PengPong::build_world()
{
	SCOPED_EVENT("PengPong - build world");

	constexpr float ortho_size = 20;
	constexpr float paddle_margin = 3;

	const float ortho_width = ortho_size * PengEngine::get().aspect_ratio();
	const float paddle_delta_x = ortho_width - paddle_margin;

    Texture::Config digit_config;
	digit_config.wrap_x = GL_CLAMP_TO_EDGE;
	digit_config.wrap_y = GL_CLAMP_TO_EDGE;
	digit_config.min_filter = GL_NEAREST;
	digit_config.max_filter = GL_NEAREST;
	digit_config.generate_mipmaps = false;

	std::vector<peng::shared_ref<const Sprite>> digit_sprites;
	for (uint8_t i = 0; i < 10; i++)
	{
		peng::shared_ref<const Texture> texture = peng::make_shared<Texture>(
			strtools::catf("Digit%d", i),
			strtools::catf("resources/textures/demo/digits/%d.png", i),
			digit_config
		);

		peng::shared_ref<const Sprite> sprite = peng::make_shared<Sprite>(
			texture, 9
		);

		digit_sprites.push_back(sprite);
	}

	// TODO: PengEngine::get().entity_manager().create_entity is far too long
	peng::weak_ptr<Camera> camera = PengEngine::get().entity_manager().create_entity<Camera>();
	camera->make_orthographic(ortho_size, 0.01f, 100.0f);

	peng::weak_ptr<Paddle> paddle_1 = PengEngine::get().entity_manager().create_entity<Paddle>("Paddle1");
	paddle_1->input_axis.positive = input::KeyCode::w;
	paddle_1->input_axis.negative = input::KeyCode::s;
	paddle_1->local_transform().position = Vector3f(-paddle_delta_x, 0, 0);

	peng::weak_ptr<Paddle> paddle_2 = PengEngine::get().entity_manager().create_entity<Paddle>("Paddle2");
	paddle_2->input_axis.positive = input::KeyCode::up;
	paddle_2->input_axis.negative = input::KeyCode::down;
	paddle_2->local_transform().position = Vector3f(+paddle_delta_x, 0, 0);

	constexpr float digit_size = 5;

	peng::weak_ptr<NumberDisplay> score_1 = PengEngine::get().entity_manager().create_entity<NumberDisplay>("Score1");
	score_1->local_transform().scale = Vector3f::one() * digit_size;
	score_1->local_transform().position = Vector3f(-digit_size * 2, ortho_size * 0.75f, -5);
	score_1->set_digit_sprites(digit_sprites);

	peng::weak_ptr<NumberDisplay> score_2 = PengEngine::get().entity_manager().create_entity<NumberDisplay>("Score2");
	score_2->local_transform().scale = Vector3f::one() * 5;
	score_2->local_transform().position = Vector3f(digit_size * 2, ortho_size * 0.75f, -5);
	score_2->set_digit_sprites(digit_sprites);

	// TODO: should use a subscribe_weak for better safety
	paddle_1->on_score_changed().subscribe([score_1](int32_t score)
		{
			score_1->set_number(score);
		});

	paddle_2->on_score_changed().subscribe([score_2](int32_t score)
		{
			score_2->set_number(score);
		});

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

	peng::weak_ptr<Entity> background = PengEngine::get().entity_manager().create_entity<Entity>("Background");

	const Vector3f stripe_size = Vector3f(0.5f, 1.5f, 1);
	const float stripe_padding = 1.5f;
	const float stripe_spacing = stripe_size.y + stripe_padding;
	const int32_t num_stripes = static_cast<int32_t>(ortho_size / stripe_spacing);

	for (int i = -num_stripes; i <= num_stripes; i++)
	{
		peng::weak_ptr<Entity> stripe = PengEngine::get().entity_manager().create_entity<Entity>("Stripe");

		stripe->local_transform().scale = stripe_size;
		stripe->local_transform().position = Vector3f(0, i * stripe_spacing, 0);
		stripe->set_parent(background, EntityRelationship::none);
		stripe->add_component<components::SpriteRenderer>();
	}
}
