#pragma once

#include <core/component.h>
#include <rendering/mesh.h>
#include <rendering/material.h>

namespace entities
{
	class PointLight;
}

namespace components
{
	class MeshRenderer final : public Component
	{
		DECLARE_COMPONENT(MeshRenderer);

	public:
		MeshRenderer(
			peng::shared_ref<const rendering::Mesh>&& mesh,
			peng::shared_ref<rendering::Material>&& material
		);

		MeshRenderer(
			const peng::shared_ref<const rendering::Mesh>& mesh,
			const peng::shared_ref<rendering::Material>& material
		);

		void tick(float delta_time) override;
		void post_create() override;

		[[nodiscard]] const peng::shared_ref<const rendering::Mesh>& mesh() const noexcept { return _mesh; }
		[[nodiscard]] const peng::shared_ref<rendering::Material>& material() const noexcept { return _material; }

	private:
		std::vector<peng::shared_ref<const entities::PointLight>> get_relevant_point_lights();

		peng::shared_ref<const rendering::Mesh> _mesh;
		peng::shared_ref<rendering::Material> _material;

		bool _uses_lighting = false;

		struct PointLightUniformSet
		{
			int32_t pos = -1;
			int32_t color = -1;
			int32_t ambient = -1;
			int32_t range = -1;
			int32_t max_strength = -1;
		};

		int32_t _cached_model_matrix = -1;
		int32_t _cached_normal_matrix = -1;
		int32_t _cached_view_matrix = -1;
		int32_t _cached_view_pos = -1;
		
		int32_t _max_point_lights = 0;
		std::vector<PointLightUniformSet> _cached_point_light_uniform_sets;
	};
}
