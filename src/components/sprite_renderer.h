#pragma once

#include <core/component.h>

namespace rendering
{
	class Sprite;
}

namespace components
{
	// TODO: add a pixel perfect mode
	class SpriteRenderer final : public Component
	{
		DECLARE_COMPONENT(SpriteRenderer);

	public:
		SpriteRenderer();
		explicit SpriteRenderer(const peng::shared_ref<const rendering::Sprite>& sprite);

		void tick(float delta_time) override;

		[[nodiscard]] peng::shared_ref<const rendering::Sprite>& sprite() noexcept { return _sprite; }
		[[nodiscard]] const peng::shared_ref<const rendering::Sprite>& sprite() const noexcept { return _sprite; }

		[[nodiscard]] math::Vector4f& color() noexcept { return _color; }
		[[nodiscard]] const math::Vector4f& color() const noexcept { return _color; }

	private:
		peng::shared_ref<const rendering::Sprite> _sprite;
		math::Vector4f _color;
	};
}
