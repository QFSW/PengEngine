#pragma once

#include <core/component.h>

namespace components
{
	// TODO: needs to use some kind of fixed_tick instead of tick
	class RigidBody2D final : public Component
	{
		DECLARE_COMPONENT(RigidBody2D);

	public:
		RigidBody2D();

		void tick(float delta_time) override;

		math::Vector2f velocity;
	};
}
