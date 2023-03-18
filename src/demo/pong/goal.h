#pragma once

#include <core/entity.h>

namespace demo::pong
{
	class Paddle;

	class Goal final : public Entity
	{
		DECLARE_ENTITY(Goal);

	public:
		Goal();
		explicit Goal(const peng::weak_ptr<Paddle>& associated_paddle);

		const peng::weak_ptr<Paddle>& associated_paddle() const noexcept;

	private:
		peng::weak_ptr<Paddle> _associated_paddle;
	};
}
