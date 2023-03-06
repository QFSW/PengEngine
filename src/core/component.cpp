#include "component.h"

#include <cassert>

#include "logger.h"

IMPLEMENT_COMPONENT(Component);

Component::Component(TickGroup tick_group)
	: _tick_group(tick_group)
{ }

TickGroup Component::tick_group() const noexcept
{
	return _tick_group;
}

Entity& Component::owner() noexcept
{
	const Component* const_this = this;
	return const_cast<Entity&>(const_this->owner());
}

const Entity& Component::owner() const noexcept
{
	const peng::shared_ptr<Entity> owner_locked = _owner.lock();

	// If the owner is no longer valid then something has gone wrong
	// as the component should never outlive the owner
	assert(owner_locked);
	return *owner_locked.get();
}

void Component::set_owner(peng::shared_ref<Entity>&& entity)
{
	if (_owner.valid())
	{
		Logger::error("Component already has an owner");
	}
	else
	{
		_owner = entity;
	}
}
