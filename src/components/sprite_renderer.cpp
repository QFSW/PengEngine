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

	if (!Camera::current())
	{
		return;
	}

	const Matrix4x4f model_matrix = owner().transform_matrix();
	const Matrix4x4f view_matrix = Camera::current()->view_matrix();
	const Matrix4x4f mvp_matrix = view_matrix * model_matrix;

	RenderQueue::get().enqueue_command(SpriteDrawCall{
		.sprite = _sprite,
		.mvp_matrix = mvp_matrix,
		.color = _color
	});
}

void SpriteRenderer::post_create()
{
	Component::post_create();

    cache_uniforms(_material_opaque, _cached_uniforms_opaque);
	cache_uniforms(_material_transparent, _cached_uniforms_transparent);
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
