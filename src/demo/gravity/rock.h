#pragma once

#include <core/entity.h>

namespace demo::gravity
{
	class Rock final : public Entity
	{
		DECLARE_ENTITY(Rock);

	public:
		using Entity::Entity;

		void post_create() override;
		void tick(float delta_time) override;

		[[nodiscard]] float radius() const noexcept;

		float mass = 1;
		float scale = 1;

		math::Vector3f velocity;

	private:
		float _radius = 1;
	};
}
