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

		[[nodiscard]] bool has_alpha() const;

		[[nodiscard]] peng::shared_ref<const rendering::Sprite>& sprite() noexcept { return _sprite; }
		[[nodiscard]] const peng::shared_ref<const rendering::Sprite>& sprite() const noexcept { return _sprite; }

		[[nodiscard]] math::Vector4f& color() noexcept { return _color; }
		[[nodiscard]] const math::Vector4f& color() const noexcept { return _color; }

	private:
		struct UniformSet
		{
			int32_t color_tex = -1;
			int32_t base_color = -1;
			int32_t model_matrix = -1;
			int32_t view_matrix = -1;
			int32_t tex_scale = -1;
			int32_t tex_offset = -1;
		};

		void cache_uniforms(const peng::shared_ref<rendering::Material>& material, UniformSet& uniform_set);

		math::Vector4f _color;
		peng::shared_ref<const rendering::Sprite> _sprite;
		peng::shared_ref<const rendering::Mesh> _mesh;
		peng::shared_ref<rendering::Material> _material_opaque;
		peng::shared_ref<rendering::Material> _material_transparent;

		UniformSet _cached_uniforms_opaque;
		UniformSet _cached_uniforms_transparent;
	};
}
