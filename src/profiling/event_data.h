#pragma once

#include <math/vector3.h>

namespace profiling
{
	struct EventData
	{
		const char* name = nullptr;
		const char* extra = nullptr;
		math::Vector3u8 color = { 0xFF, 0xFF, 0xFF };
	};
}