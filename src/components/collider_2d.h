#pragma once

#include <core/component.h>
#include <physics/aabb.h>

namespace components
{
	class Collider2D : public Component
	{
		DECLARE_COMPONENT(Collider2D);

	public:
		Collider2D();

		static const std::vector<peng::weak_ptr<Collider2D>>& active_colliders();

		void post_create() override;
		void pre_destroy() override;

		virtual physics::AABB bounding_box() const = 0;

	private:
		static std::vector<peng::weak_ptr<Collider2D>> _active_colliders;
	};
}
