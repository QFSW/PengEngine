#pragma once

#include <core/entity.h>
#include <entities/point_light.h>
#include <components/mesh_renderer.h>

namespace demo
{
	class DemoController final : public Entity
	{
	public:
		DemoController();

		void post_create() override;
		void tick(float delta_time) override;

	private:
		float _age = 0;
		peng::weak_ptr<entities::PointLight> _light_entity;
		peng::weak_ptr<components::MeshRenderer> _light_renderer;
	};
}
