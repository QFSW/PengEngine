#include "sprite_renderer.h"

#include <core/serialized_member.h>
#include <core/asset.h>
#include <entities/camera.h>
#include <rendering/primitives.h>
#include <rendering/material.h>
#include <rendering/sprite.h>
#include <rendering/render_queue.h>

IMPLEMENT_COMPONENT(components::SpriteRenderer);

using namespace entities;
using namespace components;
using namespace rendering;
using namespace math;

SpriteRenderer::SpriteRenderer()
	: SpriteRenderer(Primitives::white_sprite())
{ }

SpriteRenderer::SpriteRenderer(const peng::shared_ref<const Sprite>& sprite)
	: Component(TickGroup::render_parallel)
    , _color(Vector4f::one())
    , _sprite(sprite)
	, _mesh(Primitives::quad())
	, _material_opaque(Primitives::unlit_material())
	, _material_transparent(Primitives::unlit_alpha_material())
{
	SERIALIZED_MEMBER(_sprite);
	SERIALIZED_MEMBER(_color);
}

void SpriteRenderer::tick(float delta_time)
{
	Component::tick(delta_time);

	const peng::shared_ref<Material>& material =
		has_alpha()
		? _material_transparent
		: _material_opaque;

	const UniformSet& cached_uniforms =
		has_alpha()
		? _cached_uniforms_opaque
		: _cached_uniforms_transparent;

	if (cached_uniforms.color_tex >= 0)
	{
		material->set_parameter(cached_uniforms.color_tex, _sprite->texture());
	}

	if (cached_uniforms.base_color >= 0)
	{
		material->set_parameter(cached_uniforms.base_color, _color);
	}

	if (cached_uniforms.model_matrix >= 0)
	{
		const Vector3f sprite_scale = Vector3f(_sprite->size(), 1);
		const Matrix4x4f sprite_matrix = Matrix4x4f::from_scale(sprite_scale);
		const Matrix4x4f model_matrix = owner().transform_matrix() * sprite_matrix;
		material->set_parameter(cached_uniforms.model_matrix, model_matrix);
	}

	if (cached_uniforms.view_matrix >= 0)
	{
		const Matrix4x4f view_matrix = Camera::current() ? Camera::current()->view_matrix() : Matrix4x4f::identity();
		material->set_parameter(cached_uniforms.view_matrix, view_matrix);
	}

	if (cached_uniforms.tex_scale >= 0 && cached_uniforms.tex_offset >= 0)
	{
		// Texture coordinates have an inverted y compared to the pixel position
	    Vector2i pos_corrected = _sprite->position();
		pos_corrected.y = _sprite->texture()->resolution().y - (pos_corrected.y + _sprite->resolution().y);

		const Vector2f texture_res = Vector2f(_sprite->texture()->resolution());
		const Vector2f tex_scale = Vector2f(_sprite->resolution()) / texture_res;
		const Vector2f tex_offset = Vector2f(pos_corrected) / texture_res;
		material->set_parameter(cached_uniforms.tex_scale, tex_scale);
		material->set_parameter(cached_uniforms.tex_offset, tex_offset);
	}

	const float z_depth = owner().world_position().z;
	const float order = material->shader()->requires_blending()
		? -z_depth
		: +z_depth;

	RenderQueue::get().enqueue_draw({
		.mesh = _mesh,
		.material = material,
		.order = order
	});
}

void SpriteRenderer::post_create()
{
	Component::post_create();

    cache_uniforms(_material_opaque, _cached_uniforms_opaque);
	cache_uniforms(_material_transparent, _cached_uniforms_transparent);
}

bool SpriteRenderer::has_alpha() const
{
	return _color.w < 0.99f || _sprite->texture()->has_alpha();
}

void SpriteRenderer::cache_uniforms(const peng::shared_ref<rendering::Material>& material, UniformSet& uniform_set)
{
	auto get_uniform_location_checked = [&](const std::string& uniform_name, const std::string& required_symbol = "")
	{
		const int32_t location = material->shader()->get_uniform_location(uniform_name);
		if (location < 0)
		{
			Logger::warning(
				"Material '%s' has no '%s' parameter%s%s so rendering may be incorrect",
				material->shader()->name().c_str(), uniform_name.c_str(),
				required_symbol.empty() ? "" : " but uses ",
				required_symbol.c_str()
			);
		}

		return location;
	};

	uniform_set.color_tex = get_uniform_location_checked("color_tex");
	uniform_set.base_color = get_uniform_location_checked("base_color");
	uniform_set.model_matrix = get_uniform_location_checked("model_matrix");
	uniform_set.view_matrix = get_uniform_location_checked("view_matrix");
	uniform_set.tex_scale = get_uniform_location_checked("tex_scale");
	uniform_set.tex_offset = get_uniform_location_checked("tex_offset");
}
