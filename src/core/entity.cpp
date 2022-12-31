#include "entity.h"

#include <cassert>

#include "peng_engine.h"

Entity::Entity(TickGroup tick_group)
	: _tick_group(tick_group)
	, _created(false)
	, _active(true)
{ }

void Entity::tick(float)
{
	assert(_tick_group != TickGroup::none);
}

TickGroup Entity::tick_group() const noexcept
{
	return _tick_group;
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

void Entity::set_parent(const peng::weak_ptr<Entity>& parent)
{
	if (parent == _parent)
	{
		return;
	}

	if (_parent.valid())
	{
		vectools::remove(_parent->_children, weak_this());
	}

	_parent = parent;

	if (_parent.valid())
	{
		_parent->_children.push_back(weak_this());
	}
}

void Entity::destroy()
{
	const peng::shared_ref<Entity> strong_this = peng::shared_ref(shared_from_this());
	PengEngine::get().entity_manager().destroy_entity(strong_this);
}

peng::weak_ptr<const Entity> Entity::weak_this() const
{
	return peng::shared_ref(shared_from_this());
}

peng::weak_ptr<Entity> Entity::weak_this()
{
	return peng::shared_ref(shared_from_this());
}

math::Matrix4x4f Entity::transform_matrix() const noexcept
{
	const math::Matrix4x4f local_matrix = _local_transform.to_matrix();

	if (_parent)
	{
		return _parent->transform_matrix() * local_matrix;
	}

	return local_matrix;
}

math::Matrix4x4f Entity::transform_matrix_inv() const noexcept
{
	const math::Matrix4x4f local_matrix_inv = _local_transform.to_inverse_matrix();

	if (_parent)
	{
		return local_matrix_inv * _parent->transform_matrix_inv();
	}

	return local_matrix_inv;
}

void Entity::cleanup_killed_children()
{
	vectools::remove_all<peng::weak_ptr<Entity>>(_children, [](const peng::weak_ptr<Entity>& child) {
		return !child.valid();
	});
}
