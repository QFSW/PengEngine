#pragma once

#ifndef NO_PROFILING

#include "concat_macro.h"

namespace profiling
{
    struct [[nodiscard]] ScopedGPUEvent
    {
        explicit ScopedGPUEvent(const char* name);
        ~ScopedGPUEvent();

        ScopedGPUEvent(const ScopedGPUEvent&) = delete;
        ScopedGPUEvent(ScopedGPUEvent&&) = delete;
    };
}

#define SCOPED_GPU_EVENT(name) profiling::ScopedGPUEvent CONCAT(__PE_event_, __LINE__)(name)

#else

#define SCOPED_GPU_EVENT(name) ((void)0)

#endif
