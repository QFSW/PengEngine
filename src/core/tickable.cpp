#include "tickable.h"

std::ostream& operator<<(std::ostream& os, TickGroup tick_group)
{
	switch (tick_group)
	{
		case TickGroup::standard:		 os << "standard";			break;
		case TickGroup::physics:		 os << "physics";			break;
		case TickGroup::pre_render:		 os << "pre_render";		break;
		case TickGroup::render:			 os << "render";			break;
		case TickGroup::render_parallel: os << "render_parallel";	break;
		case TickGroup::post_render:	 os << "post_render";		break;
		case TickGroup::none:			 os << "none";				break;
		default: os << "???"; break;
	}

	return os;
}

bool is_parallel_tick_group(TickGroup tick_group)
{
	switch (tick_group)
	{
		case TickGroup::render_parallel: return true;
		default: return false;
	}
}
