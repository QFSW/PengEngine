#pragma once

#include <memory/weak_ptr.h>

#include "tickable.h"

class Entity;

class Component : public ITickable
{
	friend Entity;

public:
	explicit Component(TickGroup tick_group = TickGroup::standard);
	Component(const Component&) = delete;
	Component(Component&&) = delete;
	virtual ~Component() = default;

	void tick([[maybe_unused]] float delta_time) override { }
	[[nodiscard]] TickGroup tick_group() const noexcept override;

	virtual void post_create() { }
	virtual void pre_destroy() { }

	[[nodiscard]] Entity& owner() noexcept;
	[[nodiscard]] const Entity& owner() const noexcept;

private:
	void set_owner(peng::shared_ref<Entity>&& entity);

	TickGroup _tick_group;
	peng::weak_ptr<Entity> _owner;
};