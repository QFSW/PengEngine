#pragma once

#include <ostream>

enum class TickGroup
{
	standard,
	physics,
	pre_render,
	render,
	render_parallel,
	post_render,

	none
};

class ITickable
{
public:
	virtual void tick(float delta_time) = 0;
	[[nodiscard]] virtual TickGroup tick_group() const noexcept = 0;
};

std::ostream& operator<<(std::ostream& os, TickGroup tick_group);
bool is_parallel_tick_group(TickGroup tick_group);