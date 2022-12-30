#include "entity.h"

#include <cassert>

Entity::Entity(bool can_tick)
	: _can_tick(can_tick)
	, _created(false)
{ }

void Entity::tick(double delta_time)
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
