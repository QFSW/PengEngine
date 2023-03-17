#pragma once

#include <memory/weak_ptr.h>

#include "tickable.h"
#include "component_definition.h"

class Entity;

class Component : public ITickable, public std::enable_shared_from_this<Component>
{
	friend Entity;

	DECLARE_COMPONENT(Component);

public:
	explicit Component(TickGroup tick_group = TickGroup::standard);
	Component(const Component&) = delete;
	Component(Component&&) = delete;
	virtual ~Component() = default;

	void tick([[maybe_unused]] float delta_time) override { }
	[[nodiscard]] TickGroup tick_group() const noexcept override;

	virtual void post_create() { }
	virtual void pre_destroy() { }

	// TODO: add a deserialize function so components can extract extra information from their json def

	[[nodiscard]] Entity& owner() noexcept;
	[[nodiscard]] const Entity& owner() const noexcept;

private:
	void set_owner(peng::shared_ref<Entity>&& entity);

	TickGroup _tick_group;
	peng::weak_ptr<Entity> _owner;
};