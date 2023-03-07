#pragma once

#include <core/entity.h>

#include "rock.h"

// TODO: we need either a FixedUpdate equivalent or a way to cap tick times otherwise physics sim can break from lag spikes

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
