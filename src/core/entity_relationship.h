#pragma once

#include <utils/enum_flags.h>

enum class EntityRelationship
{
	spatial = 1 << 0,
	activity = 1 << 1,

	none = 0,
	full = ~0
};

ENUM_FLAGS(EntityRelationship)