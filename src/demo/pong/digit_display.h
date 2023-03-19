#pragma once

#include <core/entity.h>

namespace rendering
{
	class Texture;
}

namespace demo::pong
{
	class DigitDisplay final : public Entity
	{
		DECLARE_ENTITY(DigitDisplay);

	public:
        explicit DigitDisplay(const std::string& name);

		void post_create() override;

		void set_digit_textures(const std::vector<peng::shared_ref<const rendering::Texture>>& textures);
		void set_digit(uint8_t digit);

	private:
		std::vector<peng::shared_ref<const rendering::Texture>> _digit_textures;
	};
}
