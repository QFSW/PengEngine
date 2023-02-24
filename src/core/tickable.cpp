#include "tickable.h"

std::ostream& operator<<(std::ostream& os, TickGroup tick_group)
{
	switch (tick_group)
	{
		case TickGroup::standard:		os << "standard"; break;
		case TickGroup::pre_render:		os << "pre_render"; break;
		case TickGroup::render:			os << "render"; break;
		case TickGroup::post_render:	os << "post_render"; break;
		case TickGroup::none:			os << "none"; break;
		default: os << "???"; break;
	}

	return os;
}