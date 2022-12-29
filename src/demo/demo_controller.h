#pragma once

#include <core/entity.h>

namespace demo
{
	class DemoController final : public Entity
	{
	public:
		DemoController();

		void post_create() override;
		void tick(double delta_time) override;

	private:
		float _pan_speed;
		float _rot_speed;
		float _zoom_speed;
	};
}
