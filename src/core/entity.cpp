#include "entity.h"

#include <cassert>

Entity::Entity(bool can_tick)
	: _can_tick(can_tick)
{ }

void Entity::tick(double)
{
	assert(_can_tick);
}
