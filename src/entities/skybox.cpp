#include "skybox.h"

#include <entities/camera.h>
#include <rendering/render_queue.h>
#include <rendering/primitives.h>
#include <rendering/material.h>

IMPLEMENT_ENTITY(entities::Skybox);

using namespace entities;
using namespace rendering;
using namespace math;

Skybox::Skybox()
	: Entity("Skybox", TickGroup::render)
	, _mesh(Primitives::cube_uv())
	, _material(Primitives::skybox_material())
{ }

void Skybox::tick(float delta_time)
{
	Entity::tick(delta_time);

	peng::weak_ptr<Camera> camera = Camera::current();
	if (!camera)
	{
		return;
	}

	// Get the view matrix from the camera but undo translation
	const Matrix4x4f view_matrix = camera->view_matrix();
	const Matrix4x4f view_matrix_shifted = view_matrix * Matrix4x4f::from_translation(camera->world_position());

	// TODO: cache parameter name
	_material->set_parameter("view_matrix", view_matrix_shifted);

	RenderQueue::get().enqueue_draw({
		.mesh = _mesh,
		.material = _material
	});
}

peng::shared_ref<Material>& Skybox::material() noexcept
{
	return _material;
}

peng::shared_ref<const Material> Skybox::material() const noexcept
{
	return _material;
}
