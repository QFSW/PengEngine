#pragma once

#ifndef NO_PROFILING

#include "event_data.h"

namespace profiling
{
	struct [[nodiscard]] ScopedEvent
	{
		explicit ScopedEvent(const EventData& event_data);
		~ScopedEvent();

		ScopedEvent(const ScopedEvent&) = delete;
		ScopedEvent(ScopedEvent&&) = delete;
	};
}

#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define SCOPED_EVENT(id, ...) profiling::ScopedEvent CONCAT(__PE_event_, __LINE__)({id, __VA_ARGS__})

#else

#define SCOPED_EVENT(id, ...) ((void)0)

#endif
