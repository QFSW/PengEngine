#pragma once

#include <core/component.h>
#include <rendering/mesh.h>
#include <rendering/material.h>

namespace components
{
	class MeshRenderer final : public Component
	{
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
		peng::shared_ref<const rendering::Mesh> _mesh;
		peng::shared_ref<rendering::Material> _material;

		int32_t _cached_model_matrix = -1;
		int32_t _cached_view_matrix = -1;
	};
}
