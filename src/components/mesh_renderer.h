#pragma once

#include <core/component.h>

namespace entities
{
	class PointLight;
}

namespace rendering
{
	class Mesh;
	class Material;
}

namespace components
{
	class MeshRenderer final : public Component
	{
		DECLARE_COMPONENT(MeshRenderer);

	public:
		MeshRenderer();

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

		void set_mesh(const peng::shared_ptr<const rendering::Mesh>& mesh);
		void set_material(const peng::shared_ptr<rendering::Material>& material);

		[[nodiscard]] const peng::shared_ptr<const rendering::Mesh>& mesh() const noexcept { return _mesh; }
		[[nodiscard]] const peng::shared_ptr<rendering::Material>& material() const noexcept { return _material; }

	private:
		void cache_uniforms();
		std::vector<peng::shared_ref<const entities::PointLight>> get_relevant_point_lights();

		peng::shared_ptr<const rendering::Mesh> _mesh;
		peng::shared_ptr<rendering::Material> _material;

		struct PointLightUniformSet
		{
			int32_t pos = -1;
			int32_t color = -1;
			int32_t ambient = -1;
			int32_t range = -1;
			int32_t max_strength = -1;
		};

		struct DirectionalLightUniformSet
		{
			int32_t dir = -1;
			int32_t color = -1;
			int32_t ambient = -1;
			int32_t intensity = -1;
		};

		struct UniformSet
		{
			int32_t model_matrix = -1;
			int32_t normal_matrix = -1;
			int32_t view_matrix = -1;
			int32_t view_pos = -1;

			std::vector<PointLightUniformSet> point_lights;
			std::vector<DirectionalLightUniformSet> directional_lights;
		};

		bool _uses_lighting = false;
		int32_t _max_point_lights = 0;
		int32_t _max_directional_lights = 0;
		UniformSet _cached_uniforms;
	};
}
