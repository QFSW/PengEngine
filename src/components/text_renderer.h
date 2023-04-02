#pragma once

#include <core/component.h>

namespace rendering
{
	class BitmapFont;
}

namespace components
{
	// TODO: add multi line text support
	// TODO: add pivot options
	class TextRenderer final : public Component
	{
		DECLARE_COMPONENT(TextRenderer);

	public:
		TextRenderer();
		explicit TextRenderer(const peng::shared_ref<const rendering::BitmapFont>& font);

		void set_text(const std::string& str);

		[[nodiscard]] peng::shared_ref<const rendering::BitmapFont>& font() noexcept { return _font; }
		[[nodiscard]] const peng::shared_ref<const rendering::BitmapFont>& font() const noexcept { return _font; }

	private:
		// Gets the character renderer for the nth position
		peng::weak_ptr<Entity> get_nth_char(int32_t n);

		std::string _current;
		peng::shared_ref<const rendering::BitmapFont> _font;
		std::vector<peng::weak_ptr<Entity>> _chars;
		peng::weak_ptr<Entity> _char_root;
	};
}