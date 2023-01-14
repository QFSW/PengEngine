#pragma once

#include <core/entity.h>

namespace entities
{
	class PointLight final : public Entity
	{
	public:
		PointLight();
		explicit PointLight(const std::string& name);
		explicit PointLight(std::string&& name);

		static const peng::weak_ptr<PointLight>& current();

		void post_create() override;

	private:
		static peng::weak_ptr<PointLight> _current;
	};
}
