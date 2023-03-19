#pragma once

#include <core/entity.h>

namespace rendering
{
	class Sprite;
}

namespace demo::pong
{
	class DigitDisplay final : public Entity
	{
		DECLARE_ENTITY(DigitDisplay);

	public:
        explicit DigitDisplay(const std::string& name);

		void post_create() override;

		void set_digit_sprites(const std::vector<peng::shared_ref<const rendering::Sprite>>& sprites);
		void set_digit(uint8_t digit);

	private:
		uint8_t _current = 0xFF;
		std::vector<peng::shared_ref<const rendering::Sprite>> _digit_sprites;
	};
}
