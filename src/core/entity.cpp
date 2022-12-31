#include "entity.h"

#include <cassert>

#include "peng_engine.h"

Entity::Entity(bool can_tick)
	: _can_tick(can_tick)
	, _created(false)
	, _active(true)
{ }

void Entity::tick(float delta_time)
{
	assert(_can_tick);

	for (const peng::shared_ref<Component>& component : _components)
	{
		component->tick(delta_time);
	}
}

void Entity::post_create()
{
	_created = true;

	for (const peng::shared_ref<Component>& component : _deferred_components)
	{
		component->set_owner(peng::shared_ref(shared_from_this()));
		component->post_create();
	}

	_deferred_components.clear();
}

void Entity::pre_destroy()
{
	for (const peng::shared_ref<Component>& component : _components)
	{
		component->pre_destroy();
	}
}

void Entity::set_active(bool active)
{
	if (active && !_active)
	{
		_active = true;
		post_enable();
	}
	else if (!active && _active)
	{
		_active = false;
		post_disable();
	}
}

void Entity::destroy()
{
	const peng::shared_ref<Entity> strong_this = peng::shared_ref(shared_from_this());
	PengEngine::get().entity_manager().destroy_entity(strong_this);
}

math::Transform Entity::evaluate_transform() const noexcept
{
	return _local_transform;
}
