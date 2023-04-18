#pragma once

#include <core/component.h>

namespace rendering
{
	class Sprite;
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

		void tick(float delta_time) override;

		void set_text(const std::string& str);

		[[nodiscard]] peng::shared_ref<const rendering::BitmapFont>& font() noexcept { return _font; }
		[[nodiscard]] const peng::shared_ref<const rendering::BitmapFont>& font() const noexcept { return _font; }

	private:
		struct GlyphData
		{
			peng::shared_ref<const rendering::Sprite> sprite;
			math::Matrix4x4f transform;
		};

		std::string _current;
		peng::shared_ref<const rendering::BitmapFont> _font;
		std::vector<GlyphData> _current_glyphs;
	};
}