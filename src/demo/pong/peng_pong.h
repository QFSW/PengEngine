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

	private:
		void build_world();
	};
}
