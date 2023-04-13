#pragma once

#ifndef NO_PROFILING

#include "event_data.h"
#include "concat_macro.h"

namespace profiling
{
    struct [[nodiscard]] ScopedEvent
    {
        explicit ScopedEvent(const EventData& event_data);
        ~ScopedEvent();

        ScopedEvent(const ScopedEvent&) = delete;
        ScopedEvent(ScopedEvent&&) = delete;
        ScopedEvent& operator=(const ScopedEvent&) = delete;
        ScopedEvent& operator=(ScopedEvent&&) = delete;
    };
}

// Creates a scoped event marker for reporting events to external profilers
//
// Usage: SCOPED_EVENT(id, [extra], [color]);
//  - Should always be placed at the start of a scope
//  - Only 'id' is required, whereas 'extra' and 'color' are optional
//  - The 'id' *must* be static for a given line of code - for dynamic info use 'extra'
//  - Strings provided may safely point to temporaries since they will be immediately consumed by the profiler
#define SCOPED_EVENT(id, ...) profiling::ScopedEvent CONCAT(__PE_event_, __LINE__)({id, __VA_ARGS__})

#else

#define SCOPED_EVENT(id, ...) ((void)0)

#endif
