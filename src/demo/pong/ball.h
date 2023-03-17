#pragma once

#include <core/entity.h>

namespace demo::pong
{
	class Ball final : public Entity
	{
		DECLARE_ENTITY(Ball);

	public:
		Ball();
	};
}
