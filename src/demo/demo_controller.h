#pragma once

#include <core/entity.h>
#include <entities/point_light.h>
#include <components/mesh_renderer.h>

namespace demo
{
	class DemoController final : public Entity
	{
		DECLARE_ENTITY(DemoController);

	public:
		DemoController();

		void post_create() override;
		void tick(float delta_time) override;

	private:
		float _age = 0;
		std::vector<peng::weak_ptr<entities::PointLight>> _light_entities;
		std::vector<peng::weak_ptr<components::MeshRenderer>> _light_renderers;
	};
}
