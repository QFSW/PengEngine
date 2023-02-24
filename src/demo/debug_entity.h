#pragma once

#include <core/entity.h>

namespace demo
{
	class DebugEntity final : public Entity
	{
		DECLARE_ENTITY(DebugEntity);

	public:
		using Entity::Entity;
		
		void tick(float delta_time) override;
	};
}
