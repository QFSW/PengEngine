#include "mesh_renderer.h"

#include <entities/camera.h>
#include <core/logger.h>
#include <utils/utils.h>

using namespace entities;
using namespace components;
using namespace rendering;
using namespace math;

MeshRenderer::MeshRenderer(
	peng::shared_ref<const Mesh>&& mesh,
	peng::shared_ref<Material>&& material
)
	: Component(TickGroup::render)
	, _mesh(std::move(mesh))
	, _material(std::move(material))
{ }

MeshRenderer::MeshRenderer(
	const peng::shared_ref<const Mesh>& mesh,
	const peng::shared_ref<Material>& material
)
	: MeshRenderer(
		utils::copy(mesh),
		utils::copy(material)
	)
{ }

void MeshRenderer::tick(float delta_time)
{
	Component::tick(delta_time);

	if (_cached_transform_location >= 0)
	{
		const Matrix4x4f view_matrix = Camera::current() ? Camera::current()->view_matrix() : Matrix4x4f::identity();
		const Matrix4x4f transform = view_matrix * owner().transform_matrix();
		_material->set_parameter(_cached_transform_location, transform);
	}

	_material->use();
	_mesh->render();
}

void MeshRenderer::post_create()
{
	Component::post_create();

	_cached_transform_location = _material->shader()->get_uniform_location("transform");
	if (_cached_transform_location < 0)
	{
		Logger::get().logf(LogSeverity::warning, "Material has no 'transform' parameter so rendering may be incorrect");
	}
}
