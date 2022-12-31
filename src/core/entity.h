#pragma once

#include <vector>
#include <memory>

#include <memory/weak_ptr.h>
#include <math/transform.h>

#include "tickable.h"
#include "component.h"

class Entity : public ITickable, public std::enable_shared_from_this<Entity>
{
public:
	explicit Entity(bool can_tick);
	virtual ~Entity() = default;

	virtual void tick(float delta_time);
	virtual void post_create();
	virtual void pre_destroy();
	virtual void post_enable() { }
	virtual void post_disable() { }

	void set_active(bool active);
	void destroy();

	template <std::derived_from<Component> T, typename ...Args>
	peng::weak_ptr<T> add_component(Args&&...args);

	[[nodiscard]] bool can_tick() const noexcept { return _can_tick; }
	[[nodiscard]] bool is_active() const noexcept { return _active; }

	[[nodiscard]] math::Transform evaluate_transform() const noexcept;
	[[nodiscard]] math::Transform& local_transform() noexcept { return _local_transform; }
	[[nodiscard]] const math::Transform& local_transform() const noexcept { return _local_transform; }

protected:
	bool _can_tick;
	bool _created;
	bool _active;

	math::Transform _local_transform;
	std::vector<peng::shared_ref<Component>> _components;
	std::vector<peng::shared_ref<Component>> _deferred_components;
};

template <std::derived_from<Component> T, typename ... Args>
peng::weak_ptr<T> Entity::add_component(Args&&... args)
{
	peng::shared_ref<T> component = peng::make_shared<T>(std::forward<Args>(args)...);
	_components.push_back(component);

	if (_created)
	{
		component->set_owner(peng::shared_ref(shared_from_this()));
		component->post_create();
	}
	else
	{
		_deferred_components.push_back(component);
	}

	return component;
}
