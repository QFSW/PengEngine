#include "scoped_event.h"

#include "profiler_manager.h"

using namespace profiling;

ScopedEvent::ScopedEvent(const EventData& event_data)
{
	ProfilerManager::get().current_profiler()->begin_event(event_data);
}

ScopedEvent::~ScopedEvent()
{
	ProfilerManager::get().current_profiler()->end_event();
}
