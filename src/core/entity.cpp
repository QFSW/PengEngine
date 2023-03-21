#include "entity.h"

#include <cassert>
#include <utils/utils.h>

#include "entity_subsystem.h"
#include "component.h"

IMPLEMENT_ENTITY(Entity);

Entity::Entity(std::string&& name, TickGroup tick_group)
	: _name(std::move(name))
	, _tick_group(tick_group)
	, _created(false)
	, _active_self(true)
	, _active_hierarchy(true)
	, _parent_relationship(EntityRelationship::full)
{ }

Entity::Entity(const std::string& name, TickGroup tick_group)
	: Entity(utils::copy(name), tick_group)
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
	// TODO: entities should receive post_enable() when created

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
	set_active(false);

	for (const peng::shared_ref<Component>& component : _components)
	{
		component->pre_destroy();
	}

	if (_parent)
	{
		vectools::remove(_parent->_children, weak_this());
	}
}

void Entity::set_active(bool active)
{
	_active_self = active;
	propagate_active_change(true);
}

void Entity::set_parent(const peng::weak_ptr<Entity>& parent, EntityRelationship relationship)
{
	const bool was_active_hierarchy = _active_hierarchy;

	if (parent == _parent && relationship == _parent_relationship)
	{
		return;
	}

	if (_parent.valid())
	{
		vectools::remove(_parent->_children, weak_this());
		_active_hierarchy = _active_self;
	}

	_parent = parent;
	_parent_relationship = relationship;

	if (_parent.valid())
	{
		_parent->_children.push_back(weak_this());
		_active_hierarchy = has_activity_parent()
			? _active_self && _parent->active_in_hierarchy()
			: _active_self;
	}

	if (_active_hierarchy && !was_active_hierarchy)
	{
		post_enable();
	}
	else if (!_active_hierarchy && was_active_hierarchy)
	{
		post_disable();
	}
}

void Entity::destroy()
{
	for (const peng::weak_ptr<Entity>& child : _children)
	{
		child->destroy();
	}

	EntitySubsystem::get().destroy_entity(weak_this());
}

peng::weak_ptr<Component> Entity::get_component(const peng::shared_ref<const ReflectedType>& component_type)
{
	for (const peng::shared_ref<Component>& component : _components)
	{
		if (component->type() == component_type)
		{
			return component;
		}
	}

	return {};
}

peng::weak_ptr<Component> Entity::get_component_in_children(
	const peng::shared_ref<const ReflectedType>& component_type)
{
	if (peng::weak_ptr<Component> component = get_component(component_type))
	{
		return component;
	}

	for (const peng::weak_ptr<Entity>& child : _children)
	{
		if (peng::weak_ptr<Component> component = child->get_component(component_type))
		{
			return component;
		}
	}

	return {};
}

peng::weak_ptr<const Component> Entity::get_component(const peng::shared_ref<const ReflectedType>& component_type) const
{
	return const_cast<Entity*>(this)->get_component(component_type);
}

peng::weak_ptr<const Component> Entity::get_component_in_children(
	const peng::shared_ref<const ReflectedType>& component_type) const
{
	return const_cast<Entity*>(this)->get_component_in_children(component_type);
}

bool Entity::has_parent() const noexcept
{
	return _parent.valid();
}

bool Entity::has_spatial_parent() const noexcept
{
	return has_parent() && (_parent_relationship & EntityRelationship::spatial) == EntityRelationship::spatial;
}

bool Entity::has_activity_parent() const noexcept
{
	return has_parent() && (_parent_relationship & EntityRelationship::activity) == EntityRelationship::activity;
}

math::Matrix4x4f Entity::transform_matrix() const noexcept
{
	const math::Matrix4x4f local_matrix = _local_transform.to_matrix();

	if (has_spatial_parent())
	{
		return _parent->transform_matrix() * local_matrix;
	}

	return local_matrix;
}

math::Matrix4x4f Entity::transform_matrix_inv() const noexcept
{
	const math::Matrix4x4f local_matrix_inv = _local_transform.to_inverse_matrix();

	if (has_spatial_parent())
	{
		return local_matrix_inv * _parent->transform_matrix_inv();
	}

	return local_matrix_inv;
}

math::Vector3f Entity::world_position() const noexcept
{
	if (has_spatial_parent())
	{
		return transform_matrix().get_translation();
	}

	return _local_transform.position;
}

void Entity::propagate_active_change(bool parent_active)
{
	const bool new_active = parent_active && _active_self;
	const bool require_enable = new_active && !_active_hierarchy;
	const bool require_disable = !new_active && _active_hierarchy;

	for (const peng::weak_ptr<Entity>& child : _children)
	{
		if (child && child->has_activity_parent())
		{
			child->propagate_active_change(new_active);
		}
	}

	_active_hierarchy = new_active;
	if (require_enable)
	{
		post_enable();
	}
	else if (require_disable)
	{
		post_disable();
	}
}
