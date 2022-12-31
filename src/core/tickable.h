#pragma once

enum class TickGroup
{
	standard,
	pre_render,
	render,
	post_render,

	none
};

class ITickable
{
public:
	virtual void tick(float delta_time) = 0;
	[[nodiscard]] virtual TickGroup tick_group() const noexcept = 0;
};