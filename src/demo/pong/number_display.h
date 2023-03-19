#pragma once

#include <core/entity.h>

namespace rendering
{
    class Texture;
}


namespace demo::pong
{
	class DigitDisplay;

	class NumberDisplay final : public Entity
	{
		DECLARE_ENTITY(NumberDisplay);

	public:
		using Entity::Entity;

		void post_create() override;

		void set_digit_textures(const std::vector<peng::shared_ref<const rendering::Texture>>& textures);
		void set_number(int32_t number);

	private:
		peng::weak_ptr<DigitDisplay> get_nth_digit(int32_t n);

		int32_t _current = -1;
		std::vector<peng::shared_ref<const rendering::Texture>> _digit_textures;
		std::vector<peng::weak_ptr<DigitDisplay>> _digits;
	};
}