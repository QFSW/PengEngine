#include "sprite_renderer.h"

#include <entities/camera.h>
#include <rendering/primitives.h>
#include <rendering/material.h>
#include <rendering/sprite.h>
#include <rendering/render_queue.h>
#include <core/logger.h>

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
    , _sprite(sprite)
	, _mesh(Primitives::quad())
	, _material(Primitives::unlit_material())
{ }

void SpriteRenderer::tick(float delta_time)
{
	Component::tick(delta_time);

	if (_cached_uniforms.color_tex >= 0)
	{
		_material->set_parameter(_cached_uniforms.color_tex, _sprite->texture());
	}

	if (_cached_uniforms.model_matrix >= 0)
	{
		const Vector3f sprite_scale = Vector3f(_sprite->size(), 1);
		const Matrix4x4f sprite_matrix = Matrix4x4f::from_scale(sprite_scale);
		const Matrix4x4f model_matrix = owner().transform_matrix() * sprite_matrix;
		_material->set_parameter(_cached_uniforms.model_matrix, model_matrix);
	}

	if (_cached_uniforms.view_matrix >= 0)
	{
		const Matrix4x4f view_matrix = Camera::current() ? Camera::current()->view_matrix() : Matrix4x4f::identity();
		_material->set_parameter(_cached_uniforms.view_matrix, view_matrix);
	}

	if (_cached_uniforms.tex_scale >= 0 && _cached_uniforms.tex_offset >= 0)
	{
		const Vector2f texture_res = Vector2f(_sprite->texture()->resolution());
		const Vector2f tex_scale = Vector2f(_sprite->resolution()) / texture_res;
		const Vector2f tex_offset = Vector2f(_sprite->position()) / texture_res;
		_material->set_parameter(_cached_uniforms.tex_scale, tex_scale);
		_material->set_parameter(_cached_uniforms.tex_offset, tex_offset);
	}

	RenderQueue::get().enqueue_draw({
		.mesh = _mesh,
		.material = _material
	});
}

void SpriteRenderer::post_create()
{
	Component::post_create();

    cache_uniforms();
}

void SpriteRenderer::cache_uniforms()
{
	auto get_uniform_location_checked = [&](const std::string& uniform_name, const std::string& required_symbol = "")
	{
		const int32_t location = _material->shader()->get_uniform_location(uniform_name);
		if (location < 0)
		{
			Logger::warning(
				"Material '%s' has no '%s' parameter%s%s so rendering may be incorrect",
				_material->shader()->name().c_str(), uniform_name.c_str(),
				required_symbol.empty() ? "" : " but uses ",
				required_symbol.c_str()
			);
		}

		return location;
	};

	_cached_uniforms.color_tex = get_uniform_location_checked("color_tex");
	_cached_uniforms.model_matrix = get_uniform_location_checked("model_matrix");
	_cached_uniforms.view_matrix = get_uniform_location_checked("view_matrix");
	_cached_uniforms.tex_scale = get_uniform_location_checked("tex_scale");
	_cached_uniforms.tex_offset = get_uniform_location_checked("tex_offset");
}
