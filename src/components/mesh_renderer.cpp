#include "mesh_renderer.h"

#include <entities/camera.h>
#include <entities/point_light.h>
#include <rendering/primitives.h>
#include <core/logger.h>
#include <utils/utils.h>

IMPLEMENT_COMPONENT(components::MeshRenderer);

using namespace entities;
using namespace components;
using namespace rendering;
using namespace math;

MeshRenderer::MeshRenderer()
	: MeshRenderer(
		Primitives::cube(),
		Primitives::phong_material()
	)
{ }

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

	// Nothing to render without a material or mesh
	if (!(_material && _mesh))
	{
		return;
	}

	if (_cached_uniforms.model_matrix >= 0)
	{
		const Matrix4x4f model_matrix = owner().transform_matrix();
		_material->set_parameter(_cached_uniforms.model_matrix, model_matrix);

		if (_cached_uniforms.normal_matrix >= 0)
		{
			const Matrix3x3f normal_matrix = Matrix3x3f(model_matrix)
				.inverse()
				.transposed();

			_material->set_parameter(_cached_uniforms.normal_matrix, normal_matrix);
		}
	}

	if (_cached_uniforms.view_matrix >= 0)
	{
		const Matrix4x4f view_matrix = Camera::current() ? Camera::current()->view_matrix() : Matrix4x4f::identity();
		_material->set_parameter(_cached_uniforms.view_matrix, view_matrix);
	}

	if (_uses_lighting)
	{
		const Vector3f view_pos = Camera::current()
			? Camera::current()->transform_matrix().get_translation()
			: Vector3f::zero();

		_material->try_set_parameter(_cached_uniforms.view_pos, view_pos);

		const std::vector<peng::shared_ref<const PointLight>> relevant_lights = get_relevant_point_lights();
		for (int32_t i = 0; i < _max_point_lights; i++)
		{
			const Vector3f light_pos = i < relevant_lights.size()
				? relevant_lights[i]->transform_matrix().get_translation()
				: Vector3f::zero();

			const PointLight::LightData light_data = i < relevant_lights.size()
				? relevant_lights[i]->data()
				: PointLight::LightData();

			const PointLightUniformSet& uniform_set = _cached_uniforms.point_lights[i];
			_material->try_set_parameter(uniform_set.pos, light_pos);
			_material->try_set_parameter(uniform_set.color, light_data.color);
			_material->try_set_parameter(uniform_set.ambient, light_data.ambient);
			_material->try_set_parameter(uniform_set.range, light_data.range);
			_material->try_set_parameter(uniform_set.max_strength, 1.0f);
		}
	}

	_material->use();
	_mesh->render();
}

void MeshRenderer::post_create()
{
	Component::post_create();

	if (_material)
	{
		cache_uniforms();
	}
}

void MeshRenderer::set_mesh(const peng::shared_ptr<const Mesh>& mesh)
{
	_mesh = mesh;
}

void MeshRenderer::set_material(const peng::shared_ptr<Material>& material)
{
	const bool material_changed = material != _material;
	_material = material;

	if (_material && material_changed)
	{
		cache_uniforms();
	}
}

void MeshRenderer::cache_uniforms()
{
	assert(_material);

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

	_cached_uniforms.model_matrix = get_uniform_location_checked("model_matrix");
	_cached_uniforms.view_matrix = get_uniform_location_checked("view_matrix");

	_uses_lighting = _material->shader()->has_symbol("SHADER_LIT");

	if (_uses_lighting)
	{
		_cached_uniforms.normal_matrix = get_uniform_location_checked("normal_matrix", "SHADER_LIT");
		_cached_uniforms.view_pos = get_uniform_location_checked("view_pos", "SHADER_LIT");

		const std::optional<std::string> max_num_lights_raw = _material->shader()->get_symbol_value("MAX_POINT_LIGHTS");
		if (max_num_lights_raw)
		{
			_max_point_lights = std::stoi(*max_num_lights_raw);
			for (int32_t i = 0; i < _max_point_lights; i++)
			{
				PointLightUniformSet uniform_set;
				uniform_set.pos = get_uniform_location_checked(strtools::catf("point_lights[%d].pos", i), "SHADER_LIT");
				uniform_set.color = get_uniform_location_checked(strtools::catf("point_lights[%d].color", i), "SHADER_LIT");
				uniform_set.ambient = get_uniform_location_checked(strtools::catf("point_lights[%d].ambient", i), "SHADER_LIT");
				uniform_set.range = get_uniform_location_checked(strtools::catf("point_lights[%d].range", i), "SHADER_LIT");
				uniform_set.max_strength = get_uniform_location_checked(strtools::catf("point_lights[%d].max_strength", i), "SHADER_LIT");

				_cached_uniforms.point_lights.push_back(uniform_set);
			}
		}
		else
		{
			Logger::warning(
				"Material '%s' has uses SHADER_LIT but has no defined value for MAX_POINT_LIGHTS",
				_material->shader()->name().c_str()
			);
		}
	}
}

std::vector<peng::shared_ref<const PointLight>> MeshRenderer::get_relevant_point_lights()
{
	struct Consideration
	{
		peng::shared_ref<const PointLight> light;
		float relevance;
	};

	// Start with all active point lights
	const std::vector<peng::weak_ptr<PointLight>> active_lights = PointLight::active_lights();

	// Calculate the relative strength for each light to the origin of this object
	// Drop any invalid or disabled lights
	// TODO: consider relative strength to bounding box instead
	// TODO: skip considerations if we don't need to do them
	std::vector<Consideration> considerations;
	for (const peng::weak_ptr<PointLight>& light : active_lights)
	{
		if (light && light->active_in_hierarchy())
		{
			const float light_intensity_sqr = light->data().color.magnitude_sqr() * light->data().range * light->data().range;
			const float light_dist_sqr = (light->world_position() - owner().world_position()).magnitude_sqr();
			const float relative_strength = light_intensity_sqr / light_dist_sqr;

			considerations.emplace_back(Consideration{
				.light = light.lock().to_shared_ref(),
				.relevance = relative_strength
			});
		}
	}

	// Sort by relevance
	std::ranges::sort(considerations, [](const auto& x, const auto& y)
	{
		return x.relevance > y.relevance;
	});

	// Only pick the most relevant ones
	std::vector<peng::shared_ref<const PointLight>> relevant_lights;
	for (size_t i = 0; i < std::min<size_t>(considerations.size(), _max_point_lights); i++)
	{
		relevant_lights.push_back(considerations[i].light);
	}

	return relevant_lights;
}
