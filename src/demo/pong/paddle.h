#pragma once

#include <core/entity.h>
#include <input/axis.h>
#include <components/box_collider_2d.h>

namespace demo::pong
{
	class Paddle final : public Entity
	{
		DECLARE_ENTITY(Paddle);

		DEFINE_EVENT(on_score_changed, int32_t);

	public:
		explicit Paddle(const std::string& name);

		void tick(float delta_time) override;
		void score_point();

		input::Axis input_axis;

		float movement_speed = 35;
		float attack_arc = 90;

	private:
		void handle_collision(peng::weak_ptr<components::Collider2D> collider);

		int32_t _score = 0;
	};
}
