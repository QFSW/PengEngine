#pragma once

#include <core/entity.h>
#include <input/axis.h>

namespace demo::pong
{
	class Paddle final : public Entity
	{
		DECLARE_ENTITY(Paddle);

	public:
		explicit Paddle(const std::string& name);

		void tick(float delta_time) override;

		input::Axis input_axis;
		float movement_speed = 30;
	};
}
