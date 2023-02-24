#pragma once

#include "event_data.h"

namespace profiling
{
	class IProfiler
	{
	public:
		virtual ~IProfiler() = default;

		virtual void begin_event(const EventData& event) = 0;
		virtual void end_event() = 0;
	};
}
