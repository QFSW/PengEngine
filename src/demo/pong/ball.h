#pragma once

#include <core/entity.h>

namespace components
{
	class Collider2D;
}

namespace demo::pong
{
	class Ball final : public Entity
	{
		DECLARE_ENTITY(Ball);

	public:
		Ball();

	private:
		void respawn();
		void handle_collision(peng::weak_ptr<components::Collider2D> collider);

		float _speed;
	};
}
