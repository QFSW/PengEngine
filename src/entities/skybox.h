#pragma once

#include <core/entity.h>

namespace entities
{
	// TODO: implement this
	class Skybox final : public Entity
	{
		DECLARE_ENTITY(Skybox);

	public:
		Skybox();

		void tick(float delta_time) override;
	};
}
