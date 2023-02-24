#pragma once

#include "profiler.h"

namespace profiling
{
	class StubProfiler final : public IProfiler
	{
	public:
		void begin_event(const EventData&) override {}
		void end_event() override {}
	};
}