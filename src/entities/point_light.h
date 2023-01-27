#pragma once

#include <core/entity.h>

namespace entities
{
	class PointLight final : public Entity
	{
	public:
		struct LightData
		{
			math::Vector3f color = math::Vector3f::zero();
			math::Vector3f ambient = math::Vector3f::zero();
			float range = 1;
		};

		PointLight();
		explicit PointLight(const std::string& name);
		explicit PointLight(std::string&& name);

		static const std::vector<peng::weak_ptr<PointLight>>& active_lights();

		void post_create() override;
		void pre_destroy() override;

		[[nodiscard]] LightData& data() noexcept;
		[[nodiscard]] const LightData& data() const noexcept;

	private:
		static std::vector<peng::weak_ptr<PointLight>> _active_lights;

		LightData _data;
	};
}
