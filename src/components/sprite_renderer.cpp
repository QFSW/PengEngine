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
	, _sprite(sprite)
    , _color(Vector4f::one())
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
