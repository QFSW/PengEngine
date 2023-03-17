#pragma once

#include <core/component.h>

namespace components
{
	// TODO: needs to use some kind of fixed_tick instead of tick
	class RigidBody final : public Component
	{
		DECLARE_COMPONENT(RigidBody);

	public:
		RigidBody();

		void tick(float delta_time) override;

		math::Vector3f velocity;
	};
}
