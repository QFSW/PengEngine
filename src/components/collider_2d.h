#pragma once

#include <core/component.h>
#include <physics/aabb.h>
#include <utils/event.h>

namespace components
{
	class Collider2D : public Component
	{
		DECLARE_COMPONENT(Collider2D);

		DEFINE_EVENT(on_trigger_enter, const peng::weak_ptr<Collider2D>&)
		DEFINE_EVENT(on_trigger_stay, const peng::weak_ptr<Collider2D>&)
		DEFINE_EVENT(on_trigger_exit, const peng::weak_ptr<Collider2D>&)

	public:
		Collider2D();

		static const std::vector<peng::weak_ptr<Collider2D>>& active_colliders();

		void post_create() override;
		void pre_destroy() override;
		void tick(float delta_time) override;

		[[nodiscard]] virtual physics::AABB bounding_box() const = 0;

		[[nodiscard]] bool triggers_enabled() const noexcept;
		[[nodiscard]] bool& triggers_enabled() noexcept;

	private:
		static std::vector<peng::weak_ptr<Collider2D>> _active_colliders;

		bool _triggers_enabled;
		std::vector<peng::weak_ptr<Collider2D>> _current_overlaps;
	};
}
