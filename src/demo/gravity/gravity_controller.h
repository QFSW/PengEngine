#pragma once

#include <core/entity.h>

#include "rock.h"

namespace demo::gravity
{
	class GravityController final : public Entity
	{
		DECLARE_ENTITY(GravityController);

	public:
		using Entity::Entity;

		void post_create() override;
		void tick(float delta_time) override;

	private:
		void create_rock_field(int32_t count, float radius, float speed);

		std::vector<peng::weak_ptr<Rock>> _rocks;
	};
}
