#pragma once

#include <core/entity.h>

namespace demo::pong
{
	class PengPong final : public Entity
	{
		DECLARE_ENTITY(PengPong);

	public:
		using Entity::Entity;

		void post_create() override;
		void tick(float delta_time) override;
	};
}
