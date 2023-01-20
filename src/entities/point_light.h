#pragma once

#include <core/entity.h>

namespace entities
{
	class PointLight final : public Entity
	{
	public:
		struct LightData
		{
			math::Vector3f color = math::Vector3f::one();
			math::Vector3f ambient = math::Vector3f::one() * 0.01f;
			float range = 10;
		};

		PointLight();
		explicit PointLight(const std::string& name);
		explicit PointLight(std::string&& name);

		static const peng::weak_ptr<PointLight>& current();

		void post_create() override;

		[[nodiscard]] LightData& data() noexcept;
		[[nodiscard]] const LightData& data() const noexcept;

	private:
		static peng::weak_ptr<PointLight> _current;

		LightData _data;
	};
}
