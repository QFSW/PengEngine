#include "mesh_renderer.h"

#include <entities/camera.h>
#include <entities/point_light.h>
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

	if (_cached_model_matrix >= 0)
	{
		const Matrix4x4f model_matrix = owner().transform_matrix();
		_material->set_parameter(_cached_model_matrix, model_matrix);

		if (_cached_normal_matrix >= 0)
		{
			const Matrix3x3f normal_matrix = Matrix3x3f(model_matrix)
				.inverse()
				.transposed();

			_material->set_parameter(_cached_normal_matrix, normal_matrix);
		}
	}

	if (_cached_view_matrix >= 0)
	{
		const Matrix4x4f view_matrix = Camera::current() ? Camera::current()->view_matrix() : Matrix4x4f::identity();
		_material->set_parameter(_cached_view_matrix, view_matrix);
	}

	if (_uses_lighting)
	{
		const Vector3f view_pos = Camera::current()
			? Camera::current()->transform_matrix().get_translation()
			: Vector3f::zero();

		const Vector3f light_pos = PointLight::current()
			? PointLight::current()->transform_matrix().get_translation()
			: Vector3f::zero();

		const PointLight::LightData light_data = PointLight::current()
			? PointLight::current()->data()
			: PointLight::LightData();

		_material->try_set_parameter(_cached_view_pos, view_pos);
		_material->try_set_parameter(_cached_light_pos, light_pos);
		_material->try_set_parameter(_cached_light_color, light_data.color);
		_material->try_set_parameter(_cached_light_ambient, light_data.ambient);
		_material->try_set_parameter(_cached_light_range, light_data.range);
	}

	_material->use();
	_mesh->render();
}

void MeshRenderer::post_create()
{
	Component::post_create();

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

	_cached_model_matrix = get_uniform_location_checked("model_matrix");
	_cached_view_matrix = get_uniform_location_checked("view_matrix");

	_uses_lighting = _material->shader()->symbols().contains("SHADER_LIT");

	if (_uses_lighting)
	{
		_cached_normal_matrix = get_uniform_location_checked("normal_matrix", "SHADER_LIT");
		_cached_view_pos = get_uniform_location_checked("view_pos", "SHADER_LIT");
		_cached_light_pos = get_uniform_location_checked("light_pos", "SHADER_LIT");
		_cached_light_color = get_uniform_location_checked("light_color", "SHADER_LIT");
		_cached_light_ambient = get_uniform_location_checked("light_ambient", "SHADER_LIT");
		_cached_light_range = get_uniform_location_checked("light_range", "SHADER_LIT");
	}
}
