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
		std::vector<peng::weak_ptr<Rock>> _rocks;
	};
}
