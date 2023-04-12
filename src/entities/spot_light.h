#pragma once

#include <core/entity.h>

namespace entities
{
	// TODO: Pretty much all of this is copy and paste from point light but with different data
	//       make a template base that can be used like Light<PointLightData> for the 3 light kinds
	class SpotLight final : public Entity
	{
		DECLARE_ENTITY(SpotLight);

	public:
		struct LightData
		{
			math::Vector3f color = math::Vector3f::zero();
			math::Vector3f ambient = math::Vector3f::zero();
			float range = 1;

			// Inner angle of the spotlight cone in degrees
			float umbra = 23;

			// Outer angle of the spotlight cone in degrees
			float penumbra = 25;
		};

		SpotLight();
		explicit SpotLight(const std::string& name);
		explicit SpotLight(std::string&& name);

		static const std::vector<peng::weak_ptr<SpotLight>>& active_lights();

		void post_create() override;
		void pre_destroy() override;

		[[nodiscard]] LightData& data() noexcept;
		[[nodiscard]] const LightData& data() const noexcept;

	private:
		static std::vector<peng::weak_ptr<SpotLight>> _active_lights;

		LightData _data;
	};
}
