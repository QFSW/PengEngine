#pragma once

#include <libs/superluminal/PerformanceAPI_capi.h>

#include "profiler.h"

namespace profiling
{
    class SuperluminalProfiler final : public IProfiler
    {
    public:
        SuperluminalProfiler();
        ~SuperluminalProfiler() override;

        // Disable copying/moving
        SuperluminalProfiler(const SuperluminalProfiler&) = delete;
        SuperluminalProfiler(SuperluminalProfiler&&) = delete;

        void begin_event(const EventData& event) override;
        void end_event() override;

    private:
        // void* so we can avoid including windows.h in the header
        void* _module = nullptr;
        PerformanceAPI_Functions _functions;
    };
}
