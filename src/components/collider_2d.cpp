#include "collider_2d.h"

IMPLEMENT_COMPONENT(components::Collider2D);

using namespace components;
using namespace math;

std::vector<peng::weak_ptr<Collider2D>> Collider2D::_active_colliders;

Collider2D::Collider2D()
	: Component(TickGroup::physics)
	, _triggers_enabled(false)
{ }

const std::vector<peng::weak_ptr<Collider2D>>& Collider2D::active_colliders()
{
	return _active_colliders;
}

void Collider2D::post_create()
{
	Component::post_create();

	_active_colliders.push_back(weak_this());
}

void Collider2D::pre_destroy()
{
	Component::pre_destroy();

	vectools::remove(_active_colliders, weak_this());
}

void Collider2D::tick(float delta_time)
{
	Component::tick(delta_time);

	if (_triggers_enabled)
	{
		const physics::AABB aabb = bounding_box();
		std::vector<peng::weak_ptr<Collider2D>> old_overlaps = std::move(_current_overlaps);

		// Check all other colliders for new overlaps
		for (peng::weak_ptr<Collider2D> other : active_colliders())
		{
			if (this != other.lock().get())
			{
				const physics::AABB other_aabb = other->bounding_box();
				if (aabb.overlaps(other_aabb))
				{
					// Add to the new overlap list if successful
					_current_overlaps.push_back(other);

					if (const auto it = std::ranges::find(old_overlaps, other); it != old_overlaps.end())
					{
						// If this overlap already exists mark it as such
						old_overlaps.erase(it);
					}
					else
					{
						// Trigger enter event for new overlaps
						_on_trigger_enter(other);
					}

					// Trigger stay event for all overlaps
					_on_trigger_stay(other);
				}
			}
		}

		// Anything remaining in old_overlaps list is gone and needs an exit event
		for (peng::weak_ptr<Collider2D> collider : old_overlaps)
		{
			_on_trigger_exit(collider);
		}
	}
}

bool Collider2D::triggers_enabled() const noexcept
{
	return _triggers_enabled;
}

bool& Collider2D::triggers_enabled() noexcept
{
	return _triggers_enabled;
}
