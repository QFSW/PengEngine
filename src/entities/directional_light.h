#pragma once

#include <core/entity.h>

namespace entities
{
	class DirectionalLight final : public Entity
	{
		DECLARE_ENTITY(DirectionalLight);

	public:
		struct LightData
		{
			math::Vector3f color = math::Vector3f::zero();
			math::Vector3f ambient = math::Vector3f::zero();
			float intensity = 1;
		};

		DirectionalLight();
		explicit DirectionalLight(const std::string& name);
		explicit DirectionalLight(std::string&& name);

		static const peng::weak_ptr<DirectionalLight>& current();

		void post_create() override;

		[[nodiscard]] LightData& data() noexcept;
		[[nodiscard]] const LightData& data() const noexcept;

	private:
		static peng::weak_ptr<DirectionalLight> _current;

		LightData _data;
	};
}
