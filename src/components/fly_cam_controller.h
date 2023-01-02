#pragma once

#include <core/component.h>

namespace components
{
	class FlyCamController final : public Component
	{
	public:
		FlyCamController();

		void post_create() override;
		void tick(float delta_time) override;

	private:
		float _rot_sensitivity;
		float _fly_speed;
	};
}
