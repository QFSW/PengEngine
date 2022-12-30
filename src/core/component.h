#pragma once

#include <memory/weak_ptr.h>

class Entity;

class Component
{
	friend Entity;

public:
	Component() = default;
	Component(const Component&) = delete;
	Component(Component&&) = delete;
	virtual ~Component() = default;

	virtual void tick([[maybe_unused]] double delta_time) { }
	virtual void post_create() { }
	virtual void pre_destroy() { }

	[[nodiscard]] Entity& owner() noexcept;
	[[nodiscard]] const Entity& owner() const noexcept;

private:
	void set_owner(peng::shared_ref<Entity>&& entity);

	peng::weak_ptr<Entity> _owner;
};