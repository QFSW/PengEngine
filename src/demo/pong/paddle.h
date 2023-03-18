#pragma once

#include <core/entity.h>
#include <input/axis.h>
#include <components/box_collider_2d.h>

namespace demo::pong
{
	// TODO: stop paddle from being able to go off screen
	class Paddle final : public Entity
	{
		DECLARE_ENTITY(Paddle);

	public:
		explicit Paddle(const std::string& name);

		void tick(float delta_time) override;

		input::Axis input_axis;

		int32_t score = 0;
		float movement_speed = 35;
		float attack_arc = 90;

	private:
		void handle_collision(peng::weak_ptr<components::Collider2D> collider);
	};
}
