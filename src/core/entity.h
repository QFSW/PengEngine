#pragma once

class Entity
{
public:
	Entity(bool can_tick);

	virtual void tick(double delta_time);

	bool can_tick() const noexcept
	{
		return _can_tick;
	}

protected:
	bool _can_tick;
};

enum class EntityState
{
	Invalid,
	Valid,
	PendingAdd,
	PendingKill
};