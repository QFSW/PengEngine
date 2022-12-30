#pragma once

#include <memory>

#include "math/transform.h"

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	explicit Entity(bool can_tick);
	virtual ~Entity() = default;

	virtual void tick(double delta_time);
	virtual void post_create() { }
	virtual void pre_destroy() { }

	[[nodiscard]] bool can_tick() const noexcept { return _can_tick; }
	[[nodiscard]] math::Transform& transform() noexcept { return _transform; }
	[[nodiscard]] const math::Transform& transform() const noexcept { return _transform; }

protected:
	bool _can_tick;
	math::Transform _transform;
};

enum class EntityState
{
	invalid,
	valid,
	pending_add,
	pending_kill
};