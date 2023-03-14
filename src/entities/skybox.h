#pragma once

#include <core/entity.h>

namespace rendering
{
	class Mesh;
	class Material;
}

namespace entities
{
	// TODO: Work out why there are ugly seams on some parts of the skybox
	class Skybox final : public Entity
	{
		DECLARE_ENTITY(Skybox);

	public:
		Skybox();

		void tick(float delta_time) override;

		[[nodiscard]] peng::shared_ref<rendering::Material>& material() noexcept;
		[[nodiscard]] peng::shared_ref<const rendering::Material> material() const noexcept;

	private:
		peng::shared_ref<const rendering::Mesh> _mesh;
		peng::shared_ref<rendering::Material> _material;
	};
}
