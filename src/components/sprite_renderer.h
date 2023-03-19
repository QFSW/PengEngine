#pragma once

#include <core/component.h>

namespace rendering
{
	class Mesh;
	class Material;
	class Sprite;
}

namespace components
{
	// TODO: make a simpler sprite material to be used instead of unlit
	class SpriteRenderer final : public Component
	{
		DECLARE_COMPONENT(SpriteRenderer);

	public:
		SpriteRenderer();
		SpriteRenderer(const peng::shared_ref<const rendering::Sprite>& sprite);

		void tick(float delta_time) override;
		void post_create() override;

		[[nodiscard]] peng::shared_ref<const rendering::Sprite>& sprite() noexcept { return _sprite; }
		[[nodiscard]] const peng::shared_ref<const rendering::Sprite>& sprite() const noexcept { return _sprite; }

	private:
		void cache_uniforms();

		peng::shared_ref<const rendering::Sprite> _sprite;
		peng::shared_ref<const rendering::Mesh> _mesh;
		peng::shared_ref<rendering::Material> _material;

		struct UniformSet
		{
			int32_t color_tex = -1;
			int32_t model_matrix = -1;
			int32_t view_matrix = -1;
		};

		UniformSet _cached_uniforms;
	};
}
