#pragma once

#include <vector>
#include <memory>

#include <memory/weak_ptr.h>
#include <math/transform.h>

#include "tickable.h"
#include "serializable.h"
#include "entity_relationship.h"
#include "entity_definition.h"

class Component;

class Entity :
    public ITickable,
    public Serializable,
    public std::enable_shared_from_this<Entity>
{
	DECLARE_ENTITY(Entity);

	friend EntitySubsystem;

public:
	explicit Entity(std::string&& name, TickGroup tick_group = TickGroup::standard);
	explicit Entity(const std::string& name, TickGroup tick_group = TickGroup::standard);

	Entity(const Entity&) = delete;
	Entity(Entity&&) = delete;

	void tick(float delta_time) override;
	[[nodiscard]] TickGroup tick_group() const noexcept override;

	virtual void post_create();
	virtual void pre_destroy();
	virtual void post_enable() { }
	virtual void post_disable() { }

	void set_active(bool active);
	void set_parent(const peng::weak_ptr<Entity>& parent, EntityRelationship relationship = EntityRelationship::full);
	void add_child(const peng::weak_ptr<Entity>& child, EntityRelationship relationship = EntityRelationship::full);
	void destroy();

	template <std::derived_from<Entity> T, typename...Args>
	requires std::constructible_from<T, Args...>
	peng::weak_ptr<T> create_entity(Args&&...args);

	template <std::derived_from<Entity> T, typename...Args>
    requires std::constructible_from<T, Args...>
	peng::weak_ptr<T> create_child(Args&&...args);

	template <std::derived_from<Entity> T, EntityRelationship R, typename...Args>
    requires std::constructible_from<T, Args...>
	peng::weak_ptr<T> create_child(Args&&...args);

	template <std::derived_from<Component> T, typename...Args>
	peng::weak_ptr<T> add_component(Args&&...args);

	template <std::derived_from<Component> T>
	requires std::constructible_from<T>
	peng::weak_ptr<T> require_component();

	[[nodiscard]] peng::weak_ptr<Component> get_component(const peng::shared_ref<const ReflectedType>& component_type);
	[[nodiscard]] peng::weak_ptr<Component> get_component_in_children(const peng::shared_ref<const ReflectedType>& component_type);

	[[nodiscard]] peng::weak_ptr<const Component> get_component(const peng::shared_ref<const ReflectedType>& component_type) const;
	[[nodiscard]] peng::weak_ptr<const Component> get_component_in_children(const peng::shared_ref<const ReflectedType>& component_type) const;

	template <std::derived_from<Component> T>
	[[nodiscard]] peng::weak_ptr<T> get_component();

	template <std::derived_from<Component> T>
	[[nodiscard]] peng::weak_ptr<const T> get_component() const;

	template <std::derived_from<Component> T>
	[[nodiscard]] peng::weak_ptr<T> get_component_in_children();

	template <std::derived_from<Component> T>
	[[nodiscard]] peng::weak_ptr<const T> get_component_in_children() const;

	template <std::derived_from<Entity> T>
	[[nodiscard]] bool is_type() const;

	template <std::derived_from<Entity> T>
	[[nodiscard]] peng::weak_ptr<T> as_type();

	template <std::derived_from<Entity> T>
	[[nodiscard]] peng::weak_ptr<const T> as_type() const;

	[[nodiscard]] const std::string& name() const noexcept { return _name; }
	[[nodiscard]] bool active_in_hierarchy() const noexcept { return _active_hierarchy; }
	[[nodiscard]] bool active_self() const noexcept { return _active_self; }

	[[nodiscard]] peng::weak_ptr<Entity> parent() noexcept { return _parent; }
	[[nodiscard]] peng::weak_ptr<const Entity> parent() const noexcept { return _parent; }
	[[nodiscard]] const std::vector<peng::weak_ptr<Entity>>& children() const noexcept { return _children; }

	[[nodiscard]] bool has_parent() const noexcept;
	[[nodiscard]] bool has_spatial_parent() const noexcept;
	[[nodiscard]] bool has_activity_parent() const noexcept;

	[[nodiscard]] math::Matrix4x4f transform_matrix() const noexcept;
	[[nodiscard]] math::Matrix4x4f transform_matrix_inv() const noexcept;
	[[nodiscard]] math::Transform& local_transform() noexcept { return _local_transform; }
	[[nodiscard]] const math::Transform& local_transform() const noexcept { return _local_transform; }
	[[nodiscard]] const std::vector<peng::shared_ref<Component>>& components() const noexcept { return _components; }

	[[nodiscard]] math::Vector3f world_position() const noexcept;
	// TODO: implement world_rotation
	// TODO: implement world_scale
	// TODO: implement world_forwards
	// TODO: implement world_up
	// TODO: implement world_right

protected:
	std::string _name;
	TickGroup _tick_group;
	math::Transform _local_transform;

private:
	void propagate_active_change(bool parent_active);

	bool _constructed;
	bool _created;
	bool _active_self;
	bool _active_hierarchy;

	peng::weak_ptr<Entity> _parent;
	EntityRelationship _parent_relationship;

	std::vector<peng::weak_ptr<Entity>> _children;
	std::vector<peng::shared_ref<Component>> _components;
	std::vector<peng::shared_ref<Component>> _deferred_components;
};

template <std::derived_from<Entity> T, typename...Args>
requires std::constructible_from<T, Args...>
peng::weak_ptr<T> Entity::create_entity(Args&&...args)
{
	return EntitySubsystem::get().create_entity<T>(std::forward<Args>(args)...);
}

template <std::derived_from<Entity> T, typename...Args>
requires std::constructible_from<T, Args...>
peng::weak_ptr<T> Entity::create_child(Args&&...args)
{
	return create_child<T, EntityRelationship::full, Args...>(std::forward<Args>(args)...);
}

template <std::derived_from<Entity> T, EntityRelationship R, typename...Args>
requires std::constructible_from<T, Args...>
peng::weak_ptr<T> Entity::create_child(Args&&... args)
{
	peng::weak_ptr<T> entity = create_entity<T>(std::forward<Args>(args)...);
	add_child(entity, R);

	return entity;
}

template <std::derived_from<Component> T, typename...Args>
peng::weak_ptr<T> Entity::add_component(Args&&...args)
{
	peng::shared_ref<T> component = peng::make_shared<T>(std::forward<Args>(args)...);
	_components.push_back(component);

	if (_constructed)
	{
		component->set_owner(peng::shared_ref(shared_from_this()));
	}

	if (_created)
	{
		component->post_create();
	}
	else
	{
		_deferred_components.push_back(component);
	}

	return component;
}

template <std::derived_from<Component> T>
requires std::constructible_from<T>
peng::weak_ptr<T> Entity::require_component()
{
	if (peng::weak_ptr<T> component = get_component<T>())
	{
		return component;
	}

	return add_component<T>();
}

template <std::derived_from<Component> T>
peng::weak_ptr<T> Entity::get_component()
{
	const peng::shared_ref<const ReflectedType> reflected_type = ReflectionDatabase::get().reflect_type_checked<T>();
	const peng::weak_ptr<Component> component = get_component(reflected_type);
	return peng::weak_ptr<T>(std::static_pointer_cast<T>(component.lock().get_impl()));
}

template <std::derived_from<Component> T>
peng::weak_ptr<const T> Entity::get_component() const
{
	return const_cast<Entity*>(this)->get_component<T>();
}

template <std::derived_from<Component> T>
peng::weak_ptr<T> Entity::get_component_in_children()
{
	const peng::shared_ref<const ReflectedType> reflected_type = ReflectionDatabase::get().reflect_type_checked<T>();
	const peng::weak_ptr<Component> component = get_component_in_children(reflected_type);
	return peng::weak_ptr<T>(std::static_pointer_cast<T>(component.lock().get_impl()));
}

template <std::derived_from<Component> T>
peng::weak_ptr<const T> Entity::get_component_in_children() const
{
	return const_cast<Entity*>(this)->get_component_in_children<T>();
}

template <std::derived_from<Entity> T>
bool Entity::is_type() const
{
	const peng::shared_ref<const ReflectedType> reflected_type = ReflectionDatabase::get().reflect_type_checked<T>();
	return reflected_type == type();
}

template <std::derived_from<Entity> T>
peng::weak_ptr<T> Entity::as_type()
{
	const peng::shared_ref<const ReflectedType> reflected_type = ReflectionDatabase::get().reflect_type_checked<T>();
	if (reflected_type == type())
	{
		return peng::weak_ptr<T>(std::static_pointer_cast<T>(shared_from_this()));
	}

	return {};
}

template <std::derived_from<Entity> T>
peng::weak_ptr<const T> Entity::as_type() const
{
	return const_cast<Entity*>(this)->as_type<T>();
}
